#include "stdafx.h"
#include "MainPlayer.h"
#include "ActionSender.h"

MainPlayer::MainPlayer(int p) :padNum(p), m_camera(p){
}


MainPlayer::~MainPlayer() {
}

void MainPlayer::Update() {
	CVector2 stickInput = Pad(padNum).GetStick(enLR::L);

	CVector3 moveVec;
	//ƒJƒƒ‰‰¡•ûŒü‚ÌˆÚ“®—Ê
	moveVec += m_camera.GetRight() * stickInput.x;

	//ƒJƒƒ‰‘OŒã•ûŒü‚ÌˆÚ“®—Ê
	CVector3 front = m_camera.GetFront();
	front.y = 0;
	front.Normalize();
	moveVec += front * stickInput.y;

	ActionSender action({ moveVec.x,moveVec.z },
						Pad(padNum).GetButton(enButtonA),
						Pad(padNum).GetButton(enButtonLB1));

	CPlayer::sendAction(action);

	CPlayer::Update();

	if (Pad(padNum).GetButton(enButtonLT)) {
		m_camera.setLeft();
	} else if (Pad(padNum).GetButton(enButtonRT)) {
		m_camera.setRigth();
	}
	m_camera.SetPos(CPlayer::getPosition());
}