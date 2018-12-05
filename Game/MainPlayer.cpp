#include "stdafx.h"
#include "MainPlayer.h"
#include "ActionSender.h"

#include "Network/Network.h"

MainPlayer::MainPlayer(int p, CVector4 color, const CVector3& position)
	:padNum(p), m_camera(p, position, 100.0f), CPlayer(color, position){
	if (p == 0) {
		NewGO<NetActionSenderCaster>();
	}
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

	if (GetPhoton()->GetState() == PhotonNetworkLogic::JOINED) {
		if (padNum == 0) {
			FindGO <NetActionSenderCaster>()->SetActionSender(action);
		}
		else {
			action = FindGO<NetWorkManager>()->GetReceiveActionSender(padNum - 1);
		}
	}

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

	if (Pad(padNum).GetButton(enButtonB)) {
		if (!pushB) {
			m_camera.ChangeSlow();
			pushB = true;
		}
	} else {
		pushB = false;
	}

	m_camera.SetTarget(CPlayer::getPosition());
}