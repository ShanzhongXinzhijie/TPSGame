#include "stdafx.h"
#include "MainPlayer.h"
#include "ActionSender.h"
#include "Weapon.h"
#include "GameWaiter.h"

MainPlayer::MainPlayer(int p, Team* team, const CVector3& position)
	:
#ifdef SpritScreen
	playerNum(p),
#endif
	m_camera(playerNum, position), CPlayer(p,team, position), wepHolder(CPlayer::weapon, CPlayer::WEAPON_NUM){
}


MainPlayer::~MainPlayer() {
}

void MainPlayer::Update() {

	CVector3 moveVec;
	{
		CVector2 stickInput = Pad(playerNum).GetStick(enLR::L);
		//�J�����������̈ړ���
		moveVec += m_camera.GetRight() * stickInput.x;
		//�J�����O������̈ړ���
		CVector3 front = m_camera.GetFront();
		front.y = 0;
		front.Normalize();
		moveVec += front * stickInput.y;
	}

	bool shot = false;
	bool dash = false;

	bool weaponLeft = false;
	bool weaponRight = false;
	if (Pad(playerNum).GetButton(enButtonY)) {
		wepHolder.pushY();
		if (!GameWaiter::GetIsWait()) {
			weaponLeft = Pad(playerNum).GetDown(enButtonLB1);
			weaponRight = Pad(playerNum).GetDown(enButtonRB1);
			wepHolder.changeWeapon(weaponLeft, weaponRight);
		}
	} else {
		dash = Pad(playerNum).GetButton(enButtonLB1);
		shot = Pad(playerNum).GetButton(enButtonRB1);
	}

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
						dash,
						look,
						shot,
						Pad(playerNum).GetDown(enButtonX),
						weaponLeft,
						weaponRight);

	CPlayer::sendAction(action);

	CPlayer::Update();

	if (Pad(playerNum).GetButton(enButtonLeft)) {
		m_camera.setLeft();
	} else if (Pad(playerNum).GetButton(enButtonRight)) {
		m_camera.setRigth();
	}

	if (Pad(playerNum).GetButton(enButtonDown)) {
		m_camera.BackTurn();
	}
	m_camera.setZoomScale(weapon[activeWeapon]->getZoomScale());
	m_camera.setSlow(shot);
	m_camera.SetTarget(CPlayer::getPosition(), !CPlayer::isFlying());
}

void MainPlayer::PostRender() {
	//HP�\��
	hpbar.Draw(CPlayer::m_hp, CPlayer::maxHp);

	//�e���\��
	weapon[activeWeapon]->drawBulletCount(font);

	//��s�Q�[�W�\��
	flybar.Draw(mover.getFlyTimer(), mover.c_flyTimer);
	flybar.setPosition({ 1270.0f,120.0f });

	//���`�[�����ő����̕\��
	wchar_t countDisp[12];
	swprintf_s(countDisp, L"Zombie:%d", team->getZombieCount());
	font.Draw(countDisp, { 0.1f, 0.1f });

	wepHolder.draw();
}
