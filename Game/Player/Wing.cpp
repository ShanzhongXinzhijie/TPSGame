#include "stdafx.h"
#include "Wing.h"
#include "FlyWalker.h"

const CVector3 Wing::hight = { 0.0f , 65.0f, 0.0f };

Wing::Wing(FlyWalker & player) :player(player){
	anim[0].Load(L"Resource/animData/wing.tka", true);
	m_model.Init(L"Resource/modelData/Wing.cmo", anim, 1);
	m_model.SetScale(size);
}

Wing::~Wing() {
}

bool Wing::Start() {
	m_model.GetAnimCon().Play(0);
	return true;
}

void Wing::Update() {
	CQuaternion&& rot = player.getRotation();
	m_model.SetRot(rot);
	CVector3 hight2 = hight;
	rot.Multiply(hight2);
	m_model.SetPos(player.GetPosition() + hight2);
	if (closing) {
		size -= CVector3(openSpeed, openSpeed, openSpeed) * GetDeltaTimeSec();
		if (size.x > 0) {
			m_model.SetScale(size);
		} else {
			delete this;
		}
	}else if (size.x < 1.0f) {
		size += CVector3(openSpeed, openSpeed, openSpeed) * GetDeltaTimeSec();
		m_model.SetScale(size);
	}
}

void Wing::closeDelete() {
	closing = true;
}
