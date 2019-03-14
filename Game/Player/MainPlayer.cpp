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
	const float pushYTimeThreshold = 0.2f;
	if (Pad(playerNum).GetButton(enButtonY)) {
		//��莞�Ԓ������ŕ���z�C�[���g��
		if (m_pushYButtonTime > pushYTimeThreshold) { 
			wepHolder.pushY();
		}

		if (!GameWaiter::GetIsWait()) {
			weaponLeft = Pad(playerNum).GetDown(enButtonLB1);
			weaponRight = Pad(playerNum).GetDown(enButtonRB1);
			wepHolder.changeWeapon(weaponLeft, weaponRight);
		}
		
		m_pushYButtonTime += GetDeltaTimeSec();
		if (weaponLeft || weaponRight) { //LR�ł̐؂�ւ�������,�Z�������؂�ւ�����
			m_isCanShortPushY = false;
		}
	} else {
		dash = Pad(playerNum).GetButton(enButtonLB1);
		shot = Pad(playerNum).GetButton(enButtonRB1);

		//�{�^����Z���������Ƃŕ���؂�ւ�
		if (!GameWaiter::GetIsWait() && m_isCanShortPushY && m_pushYButtonTime > FLT_EPSILON && m_pushYButtonTime < pushYTimeThreshold) {
			weaponLeft = false;
			weaponRight = true;
			wepHolder.changeWeapon(weaponLeft, weaponRight);
		}
		m_pushYButtonTime = 0.0f; m_isCanShortPushY = true;
	}
	//�\���L�[�ł̕���؂�ւ�
	if (!weaponLeft && !weaponRight) {
		if (Pad(playerNum).GetDown(enButtonLeft)) { weaponLeft = true; }
		if (Pad(playerNum).GetDown(enButtonRight)) { weaponRight = true; }
		wepHolder.changeWeapon(weaponLeft, weaponRight);
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

	if (Pad(playerNum).GetDown(enButtonUp)) {
		if (!m_camera.getLeft()) {
			m_camera.setLeft();
		}
		else {
			m_camera.setRigth();
		}
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
