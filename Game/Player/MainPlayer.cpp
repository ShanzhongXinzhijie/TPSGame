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
		//カメラ横方向の移動量
		moveVec += m_camera.GetRight() * stickInput.x;
		//カメラ前後方向の移動量
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
		//一定時間長押しで武器ホイール拡大
		if (m_pushYButtonTime > pushYTimeThreshold) { 
			wepHolder.pushY();
		}

		if (!GameWaiter::GetIsWait() && m_hp != 0) {
			weaponLeft = Pad(playerNum).GetDown(enButtonLB1);
			weaponRight = Pad(playerNum).GetDown(enButtonRB1);
			wepHolder.changeWeapon(weaponLeft, weaponRight);
		}
		
		m_pushYButtonTime += GetDeltaTimeSec();
		if (weaponLeft || weaponRight) { //LRでの切り替えしたら,短く押す切り替え無効
			m_isCanShortPushY = false;
		}
	} else {
		dash = Pad(playerNum).GetButton(enButtonLB1);
		shot = Pad(playerNum).GetButton(enButtonRB1);

		//ボタンを短く押すことで武器切り替え
		if (!GameWaiter::GetIsWait()&& m_hp != 0 && m_isCanShortPushY && m_pushYButtonTime > FLT_EPSILON && m_pushYButtonTime < pushYTimeThreshold) {
			weaponLeft = false;
			weaponRight = true;
			wepHolder.changeWeapon(weaponLeft, weaponRight);
		}
		m_pushYButtonTime = 0.0f; m_isCanShortPushY = true;
	}
	//十字キーでの武器切り替え
	if (!GameWaiter::GetIsWait()&& m_hp != 0 && !weaponLeft && !weaponRight) {
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
			m_camera.setRight();
		}
	}
	if (Pad(playerNum).GetButton(enButtonDown)) {
		m_camera.BackTurn();
	}
	m_camera.setZoomScale(weapon[activeWeapon]->getZoomScale());
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
	//HP表示
	hpbar.Draw(CPlayer::m_hp, mover.getFlyTimer(), mover.isRest());

	//弾数表示
	weapon[activeWeapon]->drawBulletCount(font);

	if (!GameWaiter::GetIsWait()) {
		//自チームの眷族数の表示
		wchar_t countDisp[12];
		swprintf_s(countDisp, L"Zombie:%d", team->getZombieCount());
		font.Draw(countDisp, { 0.014f, 0.086f }, { 0,0,0,1 }, {1.2f, 1.2f});
		font.Draw(countDisp, { 0.011f, 0.083f },{ 1,1,1,1 }, { 1.2f, 1.2f });
	}

	wepHolder.draw();
}
