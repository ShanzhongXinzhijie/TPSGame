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
	m_camera(playerNum, position), CPlayer(p,team, position), wepHolder(CPlayer::weapon, CPlayer::WEAPON_NUM),
	hpbar(maxHp, mover.getFlyTimerMax()) {
	miniHpbar.setIsEnable(false);

	godPowerStarter.Init(this);
}


MainPlayer::~MainPlayer() {
}

void MainPlayer::PreUpdate() {
	CVector2 stick = Pad(playerNum).GetStick(enLR::R);
	if (isFlying()) {
		float lx = Pad(playerNum).GetStick(enLR::L).x;
		stick.x = lx;
	}
	m_camera.setRot(stick);
}
void MainPlayer::Update() {

	CVector3 moveVec;
	if (isFlying()) {
		CVector2 stickInput = Pad(playerNum).GetStick(enLR::L);
		moveVec.x = stickInput.x;
		moveVec.z = stickInput.y;
	}else {
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

	shot = Pad(0).GetButton(enButtonRB1);
	dash = Pad(0).GetButton(enButtonLB1);

	bool weaponLeft = false;
	bool weaponRight = false;
	if (!GameWaiter::GetIsWait() && m_hp != 0 && !weapon[activeWeapon]->isReloading()) {
		if (Pad(playerNum).GetTrigger(enLR::L) > 0.2f) {
			if (!LT) {
				weaponLeft = true;
				LT = true;
			}
		} else {
			LT = false;
		}
		if (Pad(playerNum).GetTrigger(enLR::R) > 0.2f) {
			if (!RT) {
				weaponRight = true;
				RT = true;
			}
		} else {
			RT = false;
		}
	}
	wepHolder.changeWeapon(weaponLeft, weaponRight);

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
			m_camera.setRight();
		}
	}
	if (Pad(playerNum).GetButton(enButtonDown)) {
		m_camera.BackTurn();
	}
	m_camera.setSlow(shot);
	CVector3 targHeight = { 0.0f ,80.0f, 0.0f };
	if (CPlayer::isFlying()) {
		targHeight.y = 40.0f;
		m_camera.setSpring(0.9f);
	} else {
		m_camera.setSpring(0.5f);
	}
	CPlayer::getRotation().Multiply(targHeight);
	m_camera.SetTarget(CPlayer::getPosition()+targHeight);
}

void MainPlayer::PostRender() {
	//HP�\��
	hpbar.Draw(CPlayer::m_hp, mover.getFlyTimer(), mover.isFlyRest());

	//�e���\��
	weapon[activeWeapon]->drawBulletCount(font);

	if (!GameWaiter::GetIsWait()) {
		//���`�[�����ő����̕\��
		wchar_t countDisp[12];
		swprintf_s(countDisp, L"Zombie:%d", getTeam()->getZombieCount());
		font.Draw(countDisp, { 0.014f, 0.086f }, { 0,0,0,1 }, {1.2f, 1.2f});
		font.Draw(countDisp, { 0.011f, 0.083f },{ 1,1,1,1 }, { 1.2f, 1.2f });
	}

	wepHolder.draw();
}
