#include "stdafx.h"
#include "Syouraidan.h"
#include "CPlayer.h"
#include "GingerGene.h"

Syouraidan::Syouraidan(const CVector3& pos, CPlayer* target, GingerGene* gg)
{
	m_pos = pos;
	m_target = target;

	m_gingerGene = gg;
	m_gingerGene->Register(this);
}
Syouraidan::~Syouraidan()
{
	m_gingerGene->Unregister(this);
}

void Syouraidan::Update(){

	const float finTime = 0.5f;

	if (m_time >= finTime) {
		m_target->Hit(CVector3::Zero(),2000);
		if (m_eff) { delete m_eff; m_eff = nullptr; }
		delete this;
		return;
	}

	m_time += GetDeltaTimeSec();
	CVector3 pos = m_pos + (m_target->getPosition() - m_pos)*min(1.0f,m_time / finTime);

	m_effCnt -= GetDeltaTimeSec();
	
	if (m_effCnt <= 0.0f) {
		if (m_eff) { delete m_eff; m_eff = nullptr; }
		m_eff = new SuicideObj::CEffekseer(L"Resource/effect/lazerCharge.efk", 100.0f, pos);
		m_effCnt = 1.6f;
	}

	if (m_eff) {
		m_eff->SetPos(pos);
	}
}
