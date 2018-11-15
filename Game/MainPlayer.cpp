#include "stdafx.h"
#include "MainPlayer.h"
#include "ActionSender.h"

MainPlayer::MainPlayer(int p) :padNum(p), m_camera(p, {0, 0, 0}, 100.0f) {

}


MainPlayer::~MainPlayer() {
}

void MainPlayer::Update() {
	CVector2 stickInput = Pad(padNum).GetStick(enLR::L);

	CVector3 moveVec;
	//カメラ横方向の移動量
	moveVec += m_camera.GetRight() * stickInput.x;

	//カメラ前後方向の移動量
	CVector3 front = m_camera.GetFront();
	front.y = 0;
	front.Normalize();
	moveVec += front * stickInput.y;

	ActionSender action({ moveVec.x,moveVec.z },
						Pad(padNum).GetButton(enButtonA),
						Pad(padNum).GetButton(enButtonLB1),
						m_camera.getLook(),
						Pad(padNum).GetButton(enButtonRB1));

	CPlayer::sendAction(action);

	CPlayer::Update();

	if (Pad(padNum).GetButton(enButtonLT)) {
		m_camera.setLeft();
	} else if (Pad(padNum).GetButton(enButtonRT)) {
		m_camera.setRigth();
	}

	if (Pad(padNum).GetButton(enButtonDown)) {
		m_camera.BackTurn();
	}



	m_camera.SetTarget(CPlayer::getPosition());
}