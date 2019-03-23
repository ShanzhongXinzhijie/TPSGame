#include "stdafx.h"
#include "Indra.h"
#include "Cplayer.h"
#include "Game.h"
#include "GingerGene.h"

Indra::Indra(CPlayer* owner, GingerGene* gg)
{
	m_owner = owner;

	m_gingergene = gg;
	m_gingergene->SetIndra(this);
	m_gingergene->Register(this);
}
Indra::~Indra()
{
	m_gingergene->SetIndra(nullptr);
	m_gingergene->Unregister(this);
}

bool Indra::Start() {
	m_game = FindGO<Game>(L"GameClass"); if (!m_game) { return false; }

	//m_model.Init(L"Resource\\modelData\\indraSky.cmo", enFbxUpAxisY);
	//m_model.SetIsMostDepth(true);
	//m_model.SetIsShadowCaster(false);
	//m_model.SetDrawPriority(DRAW_PRIORITY_MAX-1);

	m_flash.Init(L"Resource/spriteData/osirisWindow.dds");

	//SE
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_indra.wav");
	se->Play();

	return true;
}
void Indra::Update() {
	//フラッシュ
	if (m_flashTimer > 0.0f) {
		m_flashTimer -= GetDeltaTimeSec();
	}

	//m_model.SetPos(GetMainCamera()->GetPos());

	bool isRun = true;
#ifndef SpritScreen
	if(m_owner->playerNum != GetPhoton()->GetLocalPlayerNumber()){
		isRun = false;
	}
#endif
	if (isRun) {
		//一定間隔で落雷
		m_cnt++;
		if (m_cnt >= 180 * 2) {
			//ゾンビ化
			int ind = m_game->GetCitizenGene().GetCitizenNum()*CMath::RandomZeroToOne();
			Citizen* C = m_game->GetCitizenGene().GetCitizen(ind);
			if (!C->GetIsDead()) {
				CVector3 pos = C->getPos();
				C->ChangeToKenzoku(m_owner);
#ifndef SpritScreen
				//通信
				m_owner->GetNetCaster()->SendIndra(pos);
#endif
				Thunder(pos);
				m_cnt = 0;
			}
		}
	}
}

void Indra::Thunder(const CVector3& pos){
	//エフェクト
	new SuicideObj::CEffekseer(L"Resource/effect/lazerShot_lazer.efk", 1.0f, pos, CQuaternion::Identity(), { 50.0f,300.0f,50.0f });
	new SuicideObj::CEffekseer(L"Resource/effect/lazerShot_end.efk", 50.0f, pos);
	new SuicideObj::CEffekseer(L"Resource/effect/lazerShot_start.efk", 1.0f, pos + CVector3::AxisY()*3000.0f, CQuaternion::Identity(), { 450.0f,50.0f,50.0f });

	//SE
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_thunder.wav");
	se->SetPos(pos + CVector3::AxisY()*3000.0f);//音の位置
	se->SetDistance(50000.0f);//音が聞こえる範囲
	se->Play(true);
}

void Indra::PostRender() {
	if (m_flashTimer > 0.0f) {
		m_flash.Draw({ 0.5f,0.5f }, { GetGraphicsEngine().GetFrameBuffer_W() / 240.0f,GetGraphicsEngine().GetFrameBuffer_H() / 169.0f }, { 0.5f,0.5f }, 0.0f, { 1000.0f,1000.0f ,1000.0f ,1.0f });
		return;
	}
}