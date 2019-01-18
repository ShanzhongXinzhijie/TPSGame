#include "stdafx.h"
#include "MainPlayer.h"
#include "ActionSender.h"

MainPlayer::MainPlayer(int p, Team* team, const CVector3& position)
	:
#ifdef SpritScreen
	playerNum(p),
#endif
	m_camera(playerNum, position), CPlayer(p,team, position){
}


MainPlayer::~MainPlayer() {
}

void MainPlayer::Update() {
	CVector2 stickInput = Pad(playerNum).GetStick(enLR::L);

	CVector3 moveVec;
	//カメラ横方向の移動量
	moveVec += m_camera.GetRight() * stickInput.x;

	//カメラ前後方向の移動量
	CVector3 front = m_camera.GetFront();
	front.y = 0;
	front.Normalize();
	moveVec += front * stickInput.y;

	bool shot = Pad(playerNum).GetButton(enButtonRB1);
	CVector3 look = { 0, 0, 0 };
	if (shot) {
		CVector3 pos = CPlayer::getPosition();
		pos.y += 60;
		look = m_camera.getLook(pos);
	}
	ActionSender action({ moveVec.x,moveVec.z },
						Pad(playerNum).GetButton(enButtonA),
						Pad(playerNum).GetButton(enButtonLB1),
						look,
						shot,
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
	swprintf_s(countDisp, L"眷族%d体", team->getZombieCount());
	font.Draw(countDisp, { 0.1f, 0.1f });
}
