#include "stdafx.h"
#include "MainPlayer.h"
#include "ActionSender.h"

MainPlayer::MainPlayer(int p, Team* team, const CVector3& position)
	:playerNum(p), m_camera(playerNum, position, 100.0f), CPlayer(p,team, position){
}


MainPlayer::~MainPlayer() {
}

void MainPlayer::Update() {
	CVector2 stickInput = Pad(playerNum).GetStick(enLR::L);

	CVector3 moveVec;
	//ƒJƒƒ‰‰¡•ûŒü‚ÌˆÚ“®—Ê
	moveVec += m_camera.GetRight() * stickInput.x;

	//ƒJƒƒ‰‘OŒã•ûŒü‚ÌˆÚ“®—Ê
	CVector3 front = m_camera.GetFront();
	front.y = 0;
	front.Normalize();
	moveVec += front * stickInput.y;

	ActionSender action({ moveVec.x,moveVec.z },
						Pad(playerNum).GetButton(enButtonA),
						Pad(playerNum).GetButton(enButtonLB1),
						m_camera.getLook(),
						Pad(playerNum).GetButton(enButtonRB1),
						Pad(playerNum).GetDown(enButtonX));

	CPlayer::sendAction(action);

	CPlayer::Update();

	if (Pad(playerNum).GetButton(enButtonLT)) {
		m_camera.setLeft();
	} else if (Pad(playerNum).GetButton(enButtonRT)) {
		m_camera.setRigth();
	}

	if (Pad(playerNum).GetButton(enButtonDown)) {
		m_camera.BackTurn();
	}

	if (Pad(playerNum).GetDown(enButtonB)) {
		m_camera.ChangeSlow();
	}

	m_camera.SetTarget(CPlayer::getPosition());
}

void MainPlayer::PostRender() {
	hpbar.Draw(CPlayer::m_hp, CPlayer::constHp);

	wchar_t countDisp[8];
	swprintf_s(countDisp, L"áÅ‘°%d‘Ì", team->getZombieCount());
	font.Draw(countDisp, { 0.1f, 0.1f });
}
