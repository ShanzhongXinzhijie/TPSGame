#include "stdafx.h"
#include "CPlayer.h"
#include "BatBullet.h"
#include "GameWaiter.h"
#include "Wing.h"
#include "HandGun.h"
#include "Rifle.h"

CPlayer::CPlayer(int pNum,Team* tem, const CVector3& position)
	: playerNum(pNum), team(tem){
	team->addPlayer(this);
	mover.SetPosition(position);
}

CPlayer::~CPlayer() {
	delete wing;
}

bool CPlayer::Start() {
	m_animationClips[anim_run].Load(L"Resource/animData/PlayerRun.tka", true);
	m_animationClips[anim_walk].Load(L"Resource/animData/PlayerWalk.tka", true);
	m_animationClips[anim_fly].Load(L"Resource/animData/PlayerFly.tka", true);
	m_animationClips[anim_idle].Load(L"Resource/animData/PlayerIdle.tka", true);
	m_animationClips[anim_shot].Load(L"Resource/animData/PlayerShot.tka", true);
	m_animationClips[anim_reload].Load(L"Resource/animData/PlayerReload.tka", false);
	m_animationClips[anim_jump].Load(L"Resource/animData/PlayerJump.tka", true);
	m_animationClips[anim_fall].Load(L"Resource/animData/PlayerFall.tka", true);

	m_model.Init(L"Resource/modelData/PlayerVanp.cmo", m_animationClips, anim_num);

	m_model.GetSkinModel().FindMaterialSetting([&](MaterialSetting* mat) {
		mat->SetAlbedoScale(team->getColor());
	});

	mover.Init(30.0f, 40.0f, getPosition());
	CVector3 pos = getPosition();
	pos.y += mover.GetCollider()->GetHeight() / 2 + mover.GetCollider()->GetRadius();
	m_collision.CreateCapsule(pos, CQuaternion::Identity(), 30.0f, 40.0f);
	m_collision.SetName(L"CPlayer");
	m_collision.SetClass(this);

	weapon[HUND_GUN] = new HandGun(this, &m_model, anim_shot, anim_reload);
	weapon[RIFLE] = new Rifle(this, &m_model, anim_shot, anim_reload);
	activeWeapon = HUND_GUN;
	weapon[HUND_GUN]->Activate();

	return true;
};

void CPlayer::Update() {
	//ゲームがウェイト状態なら終わる
	if (GameWaiter::GetIsWait()) { return; }

	if (m_hp != 0) {
		if (!weapon[activeWeapon]->isReloading()) {
			Move();
			Shot();
			Reload();
		}
		mover.Update();
		m_model.SetPos(getPosition());
		m_model.SetRot(mover.getRotation());
		CVector3 pos = getPosition();
		pos.y += mover.GetCollider()->GetHeight()/2 + mover.GetCollider()->GetRadius();
		m_collision.SetPosition(pos);
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
		CVector3&& pos = getPosition();
		pos.y += 60.0f;
		new GameObj::Suicider::CEffekseer(L"Resource/effect/damage.efk", 1.0f, pos);
		playSE(L"Resource/sound/SE_damage.wav");
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

	const CVector2& movement = action.getMovement();

	//壁ジャンプ
	bool isWalljump = false;
	if (mover.IsContactWall()) {
		//空中に壁に当たりながらジャンプ or 飛行で壁に突っ込む
		if (!mover.IsOnGround() && action.isJump() || mover.isFlying()) {
			if (mover.isFlying()) {
				mover.flyStop();
				mover.SetFlyTimer(max(0.0f, mover.getFlyTimer() - mover.getFlyTimerMax()*0.1f));//飛行可能時間を消費
			}
			playSE(L"Resource/sound/SE_jump.wav");
			mover.walljump(jumpPower, movement);
			isWalljump = true;
		}
	}

	//ジャンプと飛行
	if (action.isJump() && !isWalljump) {
		if (mover.IsOnGround()) {
			playSE(L"Resource/sound/SE_jump.wav");
			mover.jump(jumpPower);
		}
		else{
			if (mover.isFlying()) {
				mover.flyStop();
			} else {
				mover.fly(true, action.getLookVec(), flyPower);
			}
		}
	}
	if (mover.isFlying()) {
		if (wing == nullptr) {
			wing = new Wing(mover);
		}
		mover.fly(false, action.getLookVec());
		mover.turn(action.getLookVec().x, action.getLookVec().z);
		m_model.GetAnimCon().Play(anim_fly, animInterpolateSec);
		return;
	} else {
		if (wing != nullptr) {
			wing->closeDelete();
			wing = nullptr;
		}
	}

	//移動
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
	if (mover.IsOnGround()) {
		if (movement.x != 0 || movement.y != 0) {
			if (dash) {
				m_model.GetAnimCon().Play(anim_run, animInterpolateSec);
			} else {
				m_model.GetAnimCon().Play(anim_walk, animInterpolateSec);
			}
		} else {
			m_model.GetAnimCon().Play(anim_idle, animInterpolateSec);
		}
	} else {
		if (mover.getVelocity().y > 0) {
			m_model.GetAnimCon().Play(anim_jump, animInterpolateSec);
		} else {
			m_model.GetAnimCon().Play(anim_fall, animInterpolateSec);
		}
	}

	//回転
	if (action.isShot()) {
		mover.turn(action.getLookVec().x, action.getLookVec().z);
	} else {
		mover.turn(movement.x, movement.y);
	}
}

void CPlayer::Shot() {
	if (action.isShot()) {
		weapon[activeWeapon]->shot();
	}
}

void CPlayer::Reload() {
	if (action.isReload()) {
		weapon[activeWeapon]->reload();
	}
}

void CPlayer::changeWeapon(unsigned char nextWeapon) {
	if (activeWeapon == nextWeapon || nextWeapon >= WEAPON_NUM) {
		return;
	}
	weapon[activeWeapon]->Inactivate();
	weapon[nextWeapon]->Activate();
	activeWeapon = nextWeapon;
}

void CPlayer::playSE(const wchar_t* path) {
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(path);
	se->SetPos(mover.GetPosition());//音の位置
	se->SetDistance(500.0f);//音が聞こえる範囲
	se->Play(true); //第一引数をtrue
}
