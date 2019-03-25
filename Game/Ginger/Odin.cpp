#include "stdafx.h"
#include "Odin.h"
#include "Cplayer.h"
#include "Game.h"
#include "GingerGene.h"

Odin::Odin(CPlayer* owner, GingerGene* gg, const CVector3& pos)
{
	m_owner = owner;

	m_gingergene = gg;
	m_gingergene->Register(this);

	m_pos = pos;
}
Odin::~Odin()
{
	m_gingergene->Unregister(this);
}

bool Odin::Start() {
	m_game = FindGO<Game>(L"GameClass"); if (!m_game) { return false; }

	m_odinPower.Init(m_game);

	m_model.Init(L"Resource\\modelData\\Odin.cmo", enFbxUpAxisY);
	m_model.SetPos(m_pos);
	CQuaternion rot; rot.SetRotationDeg(CVector3::AxisY(), 205.0f);
	m_model.SetRot(rot);
	m_model.SetScale(CVector3::One()*0.2f);

	//�n�`����
	m_phyStaticObject.CreateMesh(m_model);

	//�G�t�F�N�g
	SuicideObj::CEffekseer* effe = new SuicideObj::CEffekseer(L"Resource/effect/aura.efk", 1.0f, m_pos);
	effe->SetScale({ 100.0f, 100.0f ,100.0f });

	//SE
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_ginger.wav");
	se->Play();

	return true;
}
void Odin::Update() {
#ifndef SpritScreen
	//�߂��ɂ���v���C���[�ɔ\�͂�
	const std::unordered_map<int, CPlayer*>& players = m_game->getPlayers();
	for (auto& P : players) {
		if ((P.second->getPosition() - m_pos).LengthSq() < 180.0f*180.0f) {
			m_odinPower.SetTime(P.second);
		}
	}
#endif
}

bool OdinPower::Start(){
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_GREATER;
	desc.StencilEnable = false;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	GetGraphicsEngine().GetD3DDevice()->CreateDepthStencilState(&desc, &m_depthStencilState);

	//���X�^���C�U
	/*{
		D3D11_RASTERIZER_DESC desc = {};
		desc.CullMode = D3D11_CULL_FRONT;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.DepthClipEnable = true;
		desc.MultisampleEnable = true;
		desc.DepthBias = -100;
		GetGraphicsEngine().GetD3DDevice()->CreateRasterizerState(&desc, &m_RSCw);
	}
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.CullMode = D3D11_CULL_BACK;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.DepthClipEnable = true;
		desc.MultisampleEnable = true;
		desc.DepthBias = -100;
		GetGraphicsEngine().GetD3DDevice()->CreateRasterizerState(&desc, &m_RSCCw);
	}*/

	//�u�����h�X�e�[�g
	{
		D3D11_BLEND_DESC desc = {};
		desc.AlphaToCoverageEnable = true;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;// D3D11_BLEND_DEST_COLOR;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		GetGraphicsEngine().GetD3DDevice()->CreateBlendState(&desc, &m_blendState);
	}

	//�V�F�[�_
	m_ps.Load("Preset/shader/odin.fx", "PSMain", Shader::EnType::PS);

	return true;
}
void OdinPower::SetTime(CPlayer* player){	
	if(m_time[player->playerNum]<= TIME_MAX*0.6f){
		//�G�t�F�N�g
		SuicideObj::CEffekseer* effe = new SuicideObj::CEffekseer(L"Resource/effect/aura.efk", 1.0f, player->getPosition());
		effe->SetScale({ 25.0f,25.0f,25.0f });
		//SE
		SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_getPower.wav");
		se->SetPos(player->getPosition());//���̈ʒu
		se->SetDistance(500.0f);//������������͈�
		se->Play(true);
	}
	m_time[player->playerNum] = TIME_MAX;
}
void OdinPower::Update(){
	for (auto& Time : m_time) {
		if (Time > 0) {
			Time--;
		}
	}
}
void OdinPower::Render(){
	if (m_time[GetPhoton()->GetLocalPlayerNumber()] <= 0 || !m_game) { return; }

	//�����_�[�^�[�Q�b�g�Ƃ��ݒ�		
	GetEngine().GetGraphicsEngine().GetD3DDeviceContext()->OMSetRenderTargets(
		1,
		&GetEngine().GetGraphicsEngine().GetFRT().GetRTV(),
		GetEngine().GetGraphicsEngine().GetGBufferRender().GetDSV()
	);

	//�f�v�X�X�e���V���ύX
	ID3D11DepthStencilState* oldDepthStencilState = nullptr; UINT I = 0;
	GetGraphicsEngine().GetD3DDeviceContext()->OMGetDepthStencilState(&oldDepthStencilState, &I);
	//�u�����h�X�e�[�g�ύX
	ID3D11BlendState* oldBlendState = nullptr; FLOAT oldf[4]; UINT olduint;
	GetGraphicsEngine().GetD3DDeviceContext()->OMGetBlendState(&oldBlendState, oldf, &olduint);

	//�ǂ̌������̓G�`��
	const std::unordered_map<int, CPlayer*>& players = m_game->getPlayers();
	for (auto& P : players) {
		if (P.second->playerNum != GetPhoton()->GetLocalPlayerNumber()) {			
			if (P.second->GetModel().GetIsDraw()){
				//�V�F�[�_�����ւ�
				P.second->GetModel().GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
					mat->SetPS(&m_ps);
				});

				//RS�����ւ�
				//ID3D11RasterizerState* RS[2];
				//P.second->GetModel().GetSkinModel().GetRasterizerState(RS[0],RS[1]);
				//P.second->GetModel().GetSkinModel().SetRasterizerState(m_RSCw, m_RSCCw);

				//�`��
				P.second->GetModel().GetSkinModel().Draw(true, 1, m_blendState, m_depthStencilState);

				//RS�߂�
				//P.second->GetModel().GetSkinModel().SetRasterizerState(RS[0], RS[1]);

				//�V�F�[�_�߂�
				P.second->GetModel().GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
					mat->SetDefaultPS();
				});
			}
		}
	}

	//�u�����h�X�e�[�g�߂�
	GetGraphicsEngine().GetD3DDeviceContext()->OMSetBlendState(oldBlendState, oldf, olduint);
	oldBlendState->Release();
	//�f�v�X�X�e���V���߂�
	GetGraphicsEngine().GetD3DDeviceContext()->OMSetDepthStencilState(oldDepthStencilState, I);
	oldDepthStencilState->Release();

	//�����_�[�^�[�Q�b�g����
	GetEngine().GetGraphicsEngine().GetD3DDeviceContext()->OMSetRenderTargets(0, NULL, NULL);

	//���X�^���C�U�[�X�e�[�g�߂�
	GetEngine().GetGraphicsEngine().ResetRasterizerState();
}

bool OdinPowerHUD::Start(){

	m_gauge[0].Init(L"Resource/spriteData/Odinout.dds");
	m_gauge[1].Init(L"Resource/spriteData/Odinin.dds");

	return true;
}
void OdinPowerHUD::PostRender(){
	if (m_oya->GetTimePersent(GetPhoton()->GetLocalPlayerNumber()) < FLT_EPSILON) { return; }

	m_gauge[0].Draw({ 0.95f,0.2f });
	m_gauge[1].Draw({ 0.95f,0.2f }, {1.0f*m_oya->GetTimePersent(GetPhoton()->GetLocalPlayerNumber()),1.0f});
}