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

	m_model.Init(L"Resource\\modelData\\Bat.cmo", enFbxUpAxisY);
	m_model.SetPos(m_pos);

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
		if ((P.second->getPosition() - m_pos).LengthSq() < 150.0f*150.0f) {
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

	//�e�N�X�`��
	//DirectX::CreateDDSTextureFromFile(GetEngine().GetGraphicsEngine().GetD3DDevice(), L"Resource\\modelData\\whiteTex.dds", nullptr, &m_srv);
	
	//�V�F�[�_
	m_ps.Load("Preset/shader/odin.fx", "PSMain", Shader::EnType::PS);

	return true;
}
void OdinPower::SetTime(CPlayer* player){	
	if(m_time[player->playerNum]<=0){
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
	GetGraphicsEngine().GetD3DDeviceContext()->OMSetDepthStencilState(m_depthStencilState, 0);

	//�ǂ̌������̓G�`��
	const std::unordered_map<int, CPlayer*>& players = m_game->getPlayers();
	for (auto& P : players) {
		if (P.second->playerNum != GetPhoton()->GetLocalPlayerNumber()) {			
			if (P.second->GetModel().GetIsDraw()){
				//�e�N�X�`�������ւ�
				/*P.second->GetModel().GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
					mat->SetAlbedoTexture(m_srv);
				});*/

				//�V�F�[�_�����ւ�
				P.second->GetModel().GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
					mat->SetPS(&m_ps);
				});

				//�`��
				P.second->GetModel().GetSkinModel().Draw();

				//�V�F�[�_�߂�
				P.second->GetModel().GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
					mat->SetDefaultPS();
				});

				//�e�N�X�`���߂�
				/*P.second->GetModel().GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
					mat->SetDefaultAlbedoTexture();
				});*/
			}
		}
	}

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