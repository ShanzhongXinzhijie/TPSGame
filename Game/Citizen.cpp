#include "stdafx.h"
#include "Citizen.h"
#include "ICitizenBrain.h"
#include "kansen.h"

Citizen::Citizen(std::vector<CPlayer*>& ps, ICitizenBrain* moveType): players(ps){
	m_animationClips = new AnimationClip[anim_num];
	m_animationClips[anim_walk].Load(L"Resource/animData/CitizenWalk.tka", true);
	m_animationClips[anim_idle].Load(L"Resource/animData/CitizenIdle.tka", true);
	m_model = new GameObj::CSkinModelRender();
	m_model->Init(L"Resource/modelData/Citizen.cmo", m_animationClips, anim_num);
	m_model->SetPos({ 300,100,300 });

	charaCon.Init(15.0f, 80.0f, { 300,100,300 });

	m_collision.CreateCapsule(charaCon.GetPosition(), CQuaternion::Identity(), 30.0f, 80.0f);
	m_collision.SetName(L"Citizen");
	m_collision.SetClass(this);

	mover = moveType;
	Kansenzyoutai();
}

Citizen::~Citizen() {
	delete mover;
	delete m_model;
}

void Citizen::Update() {
	mover->Update(charaCon.IsOnGround());

	charaCon.GetPosition();
	//ˆÚ“®
	CVector3 moveVec = mover->getMove();
	if (moveVec.x != 0 || moveVec.z != 0) {
		m_model->GetAnimCon().Play(anim_walk);
	} else {
		m_model->GetAnimCon().Play(anim_idle);
	}
	m_model->SetPos(charaCon.Execute(moveVec));
	m_collision.SetPosition(charaCon.GetPosition());

	//‰ñ“]
	m_model->SetRot(mover->getTurn());
}

bool Citizen::BatHit(CPlayer* player, CVector3 dir) {
	if (!isKenzoku) {
		Kansenzyoutai();
	}
	return true;
}

void Citizen::Kansenzyoutai()
{
	delete m_model;
	delete[] m_animationClips;
	m_animationClips = new AnimationClip[anim_num];
	m_animationClips[anim_walk].Load(L"Resource/animData/VanpWalk.tka", true);
	m_animationClips[anim_idle].Load(L"Resource/animData/VanpIdle.tka", true);
	m_model = new GameObj::CSkinModelRender();
	m_model->Init(L"Resource/modelData/Vanp.cmo", m_animationClips, anim_num);
	m_model->SetPos(charaCon.GetPosition());

	delete mover;
	mover = new kansen(players,charaCon.GetPosition());

	isKenzoku = true;
}
