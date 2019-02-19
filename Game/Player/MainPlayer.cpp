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
	//�J�����������̈ړ���
	moveVec += m_camera.GetRight() * stickInput.x;

	//�J�����O������̈ړ���
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
	} else {
		look = m_camera.getLook();
	}
	ActionSender action({ moveVec.x,moveVec.z },
						Pad(playerNum).GetDown(enButtonA),
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
	m_camera.setSlow(shot);
	m_camera.SetTarget(CPlayer::getPosition(), !CPlayer::isFlying());
}

void MainPlayer::PostRender() {
	//HP�\��
	hpbar.Draw(CPlayer::m_hp, CPlayer::constHp);

	//�e���\��
	wchar_t countbullet[10];
	swprintf_s(countbullet, L"�c�e%d/%d��", bulletCount, constBulletCount);
	font.Draw(countbullet, { 0.7f, 0.9f });

	//��s�Q�[�W�\��
	flybar.Draw(mover.getFlyTimer(), mover.c_flyTimer);
	flybar.setPosition({ 1270.0f,120.0f });

	//���`�[�����ő����̕\��
	wchar_t countDisp[8];
	swprintf_s(countDisp, L"�ő�%d��", team->getZombieCount());
	font.Draw(countDisp, { 0.1f, 0.1f });
}
