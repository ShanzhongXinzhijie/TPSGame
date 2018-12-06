#include "stdafx.h"
#include "MainPlayer.h"
#include "ActionSender.h"

#include "Network/Network.h"

MainPlayer::MainPlayer(int p, CVector4 color, const CVector3& position)
	:playerNum(p), m_camera(0, position, 100.0f), CPlayer(p,color, position){
	if (p == 0) {
		m_actionSenderCaster = NewGO<NetActionSenderCaster>();
	}
	m_p_networkManager = FindGO<NetWorkManager>();
}


MainPlayer::~MainPlayer() {
}

void MainPlayer::Update() {
	CVector2 stickInput = Pad(0).GetStick(enLR::L);

	CVector3 moveVec;
	//ƒJƒƒ‰‰¡•ûŒü‚ÌˆÚ“®—Ê
	moveVec += m_camera.GetRight() * stickInput.x;

	//ƒJƒƒ‰‘OŒã•ûŒü‚ÌˆÚ“®—Ê
	CVector3 front = m_camera.GetFront();
	front.y = 0;
	front.Normalize();
	moveVec += front * stickInput.y;

	ActionSender action({ moveVec.x,moveVec.z },
						Pad(0).GetButton(enButtonA),
						Pad(0).GetButton(enButtonLB1),
						m_camera.getLook(),
						Pad(0).GetButton(enButtonRB1));

	if (GetPhoton()->GetState() == PhotonNetworkLogic::JOINED) {
		if (playerNum == 0) {
			m_actionSenderCaster->SetActionSender(action);
		}
		else {
			action = m_p_networkManager->GetReceiveActionSender(playerNum - 1);
		}
	}

	CPlayer::sendAction(action);

	CPlayer::Update();

	if (Pad(0).GetButton(enButtonLT)) {
		m_camera.setLeft();
	} else if (Pad(0).GetButton(enButtonRT)) {
		m_camera.setRigth();
	}

	if (Pad(0).GetButton(enButtonDown)) {
		m_camera.BackTurn();
	}

	if (Pad(0).GetButton(enButtonB)) {
		if (!pushB) {
			m_camera.ChangeSlow();
			pushB = true;
		}
	} else {
		pushB = false;
	}

	m_camera.SetTarget(CPlayer::getPosition());
}