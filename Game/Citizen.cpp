#include "stdafx.h"
#include "Citizen.h"
#include "ICitizenMover.h"

Citizen::Citizen(ICitizenMover* moveType) {
	m_animationClips[anim_walk].Load(L"Resource/animData/CitizenWalk.tka", true);
	m_animationClips[anim_idle].Load(L"Resource/animData/CitizenIdle.tka", true);
	m_model.Init(L"Resource/modelData/Citizen.cmo", m_animationClips, anim_num);
	m_model.SetPos({ 300,100,300 });

	charaCon.Init(15.0f, 80.0f, { 300,100,300 });
	mover = moveType;
}

void Citizen::Update() {
	mover->Update(charaCon.IsOnGround());

	//�ړ�
	CVector3 moveVec = mover->getMove();
	if (moveVec.x != 0 || moveVec.z != 0) {
		m_model.GetAnimCon().Play(anim_walk);
	} else {
		m_model.GetAnimCon().Play(anim_idle);
	}
	m_model.SetPos(charaCon.Execute(moveVec));

	//��]
	m_model.SetRot(mover->getTurn());
}

Citizen::~Citizen() {
	delete mover;
}