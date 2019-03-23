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

	m_model.Init(L"Resource\\modelData\\Odin.cmo", enFbxUpAxisY);
	m_model.SetPos(m_pos);

	//�G�t�F�N�g
	SuicideObj::CEffekseer* effe = new SuicideObj::CEffekseer(L"Resource/effect/aura.efk", 1.0f, m_pos);
	effe->SetScale({ 100.0f, 100.0f ,100.0f });

	//SE
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_ginger.wav");
	se->SetPos(m_pos);//���̈ʒu
	se->SetDistance(500.0f);//������������͈�
	se->Play(true);

	return true;
}
void Odin::Update() {
	//�߂��ɂ��鎩���v���C���[�ɔ\�͂�
}