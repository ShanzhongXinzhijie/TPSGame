#include "stdafx.h"
#include "Hades.h"
#include "Cplayer.h"
#include "GingerGene.h"

Hades::Hades(CPlayer* owner, GingerGene* gg)
{
	m_owner = owner;

	m_gingergene = gg;
	m_gingergene->Register(this);
}
Hades::~Hades()
{
	m_gingergene->Unregister(this);
}

bool Hades::Start(){
	//SE
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_hades.wav");
	se->Play();

	return true;
}

void Hades::Update(){
	m_owner->GetModel().SetIsDraw(false);

	/*if(m_textCnt == 0 && m_owner->GetIsDead()){
		m_textCnt = 45;
	}
	if(m_textCnt >0){
		m_textCnt--;
		if(m_textCnt<=0){
			delete this;
		}
	}*/
}

void Hades::PostRender(){
	/*if (m_textCnt > 0 && m_owner->playerNum == GetPhoton()->GetLocalPlayerNumber()) {
		m_Font.Draw(L"ƒnƒfƒX‚Ì‚©‚Ô‚Æ ‚ª ‚­‚¾‚¯‚¿‚Á‚½!", { 0.5f,0.5f }, { 1.0f,1.0f,1.0f,1.0f }, CVector2::One(), { 0.5f,0.5f }, 0.0f, DirectX::SpriteEffects_None, 0.0f);
	}*/
}