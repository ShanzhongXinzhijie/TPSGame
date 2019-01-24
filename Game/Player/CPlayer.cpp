#include "stdafx.h"
#include "CPlayer.h"
#include "BatBullet.h"

CPlayer::CPlayer(int pNum,Team* tem, const CVector3& position)
	: playerNum(pNum), team(tem){
	team->addPlayer(this);
	mover.SetPosition(position);
}

CPlayer::~CPlayer() {
}

bool CPlayer::Start() {
	m_animationClips[anim_run].Load(L"Resource/animData/PlayerRun.tka", true);
	m_animationClips[anim_walk].Load(L"Resource/animData/PlayerWalk.tka", true);
	m_animationClips[anim_fly].Load(L"Resource/animData/PlayerFly.tka", true);
	m_animationClips[anim_idle].Load(L"Resource/animData/PlayerIdle.tka", true);
	m_animationClips[anim_shot].Load(L"Resource/animData/PlayerShot.tka", true);
	m_animationClips[anim_reload].Load(L"Resource/animData/PlayerReload.tka", false);

	m_model.Init(L"Resource/modelData/PlayerVanp.cmo", m_animationClips, anim_num);

	m_model.GetAnimCon().GetAnimation(0).AddAnimationEventListener(
		[&](const wchar_t* clipName, const wchar_t* eventName) {
		if (std::wcscmp(eventName, L"Reload") == 0) {
			onReload = false;
			bulletCount = constBulletCount;
		}
	});

	m_model.GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
		mat->SetAlbedoScale(team->getColor());
	});

	mover.Init(30.0f, 90.0f, getPosition());

	m_collision.CreateCapsule(getPosition(), CQuaternion::Identity(), 30.0f, 90.0f);
	m_collision.SetName(L"CPlayer");
	m_collision.SetClass(this);

	return true;
};

void CPlayer::Update() {
	if (m_hp != 0) {
		if (!onReload) {
			Move();
			Shot();
			Reload();
		}
		mover.Update();
		m_model.SetPos(getPosition());
		m_model.SetRot(mover.getRotation());
		m_collision.SetPosition(getPosition());
	} else {
		deathCool -= GetDeltaTimeSec();
		if (deathCool <= 0.0f) {
			Revive();
		}
	}
}

CVector3 CPlayer::getPosition() const{
	return mover.GetPosition();
}

void CPlayer::sendAction(const ActionSender& actionPal) {
	action = actionPal;
}

bool CPlayer::BatHit(CPlayer* player, const CVector3& dir) {
	if (this != player) {
		Hit(dir);
		return true;
	}
	return false;
}

void CPlayer::Hit(const CVector3 & dir) {
	if (m_hp != 0) {
		mover.addVelocity(dir);
		m_hp--;
		if (m_hp == 0) {
			Death();
		}
	}
}

//死亡処理
void CPlayer::Death() {
	m_hp = 0;
	deathCool = constDeathCool;
	m_model.SetIsDraw(false);
}
//蘇生処理
void CPlayer::Revive() {
	m_hp = constHp;
	m_model.SetIsDraw(true);
}

void CPlayer::Move() {

	//ジャンプと飛行
	if (action.isJump()) {
		if (mover.IsOnGround()) {
			mover.jump(jumpPower);
		} else {
			if (mover.isFlying()) {
				mover.flyStop();
			} else {
				mover.fly(true, action.getLookVec(), flyPower);
			}
		}
	}
	if (mover.isFlying()) {
		mover.fly(false, action.getLookVec());
		mover.turn(action.getLookVec().x, action.getLookVec().z);
		m_model.GetAnimCon().Play(anim_fly, animInterpolateSec);
		return;
	}

	//移動
	const CVector2& movement = action.getMovement();

	float speed = moveSpeed;
	bool dash = false;

	//移動速度
	if (action.isShot()) {
		speed = moveSpeed * 0.5f;
	}else if(action.isDash()) {
		speed = moveSpeed * dashMul;
		dash = true;
	}

	mover.move(movement*speed);

	//アニメーション
	if (movement.x != 0 || movement.y != 0) {
		if (dash) {
			m_model.GetAnimCon().Play(anim_run, animInterpolateSec);
		} else {
			m_model.GetAnimCon().Play(anim_walk, animInterpolateSec);
		}
	} else {
		m_model.GetAnimCon().Play(anim_idle, animInterpolateSec);
	}

	//回転
	if (action.isShot()) {
		mover.turn(action.getLookVec().x, action.getLookVec().z);
	} else {
		mover.turn(movement.x, movement.y);
	}
}

void CPlayer::Shot() {
	bool shot = false;
	if (action.isShot()) {
		m_model.GetAnimCon().Play(anim_shot, animInterpolateSec);
	}
	if (shotCool <= 0) {
		if (action.isShot()) {
			shot = true;
			shotCool = constShotCool;
		}
	} else {
		shotCool -= GetDeltaTimeSec();
	}

	if (shot && bulletCount > 0) {
		CVector3 vec = action.getLookVec() * 1800;
		CVector3 pos = getPosition();
		if (mover.isFlying()) {
			vec += mover.getVelocity();
			pos += action.getLookVec() * 100;
		} else {
			pos.y += 60;
		}
		new BatBullet(this, pos, vec);
		bulletCount--;
	}
}

void CPlayer::Reload() {
	if (action.isReload() && bulletCount < constBulletCount) {
		m_model.GetAnimCon().Play(anim_reload, 0.5f);
		onReload = true;
	}
}
