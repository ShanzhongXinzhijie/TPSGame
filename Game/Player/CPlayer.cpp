#include "stdafx.h"
#include "CPlayer.h"
#include "GameWaiter.h"
#include "Wing.h"
#include "HandGun.h"
#include "Rifle.h"
#include "Lazer.h"
#include "Bullet.h"
#include "CollisionMaskConst.h"

CPlayer::CPlayer(int pNum,Team* tem, const CVector3& position)
	: playerNum(pNum), team(tem), miniHpbar(maxHp){
	team->addPlayer(this);
	mover.SetPosition(position);
#ifndef SpritScreen
	if (playerNum != GetPhoton()->GetLocalPlayerNumber()) {
		mover.SetIsLocalUser(false);
	}
#endif	
}

CPlayer::~CPlayer() {
	delete wing;
	for (Weapon* wp : weapon) {
		delete wp;
	}
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
	//マスクとグループの設定
	m_collision.All_Off_Group();
	m_collision.On_OneGroup(CollisionMaskConst::encolKurai);
	m_collision.All_Off_Mask();
	m_collision.On_OneMask(CollisionMaskConst::encolAttack);
	//これは喰らい判定
	m_collision.SetIsHurtCollision(true);

	weapon[HUND_GUN] = new HandGun(this, &m_model, anim_shot, anim_reload);
	weapon[RIFLE] = new Rifle(this, &m_model, anim_shot, anim_reload);
	weapon[LAZER] = new Lazer(this, &m_model, anim_shot, anim_reload);
	activeWeapon = HUND_GUN;
	weapon[HUND_GUN]->Activate();

	m_Init = true;
	return true;
};

void CPlayer::Update() {
	//ゲームがウェイト状態なら終わる
	if (GameWaiter::GetIsWait()) { return; }

	if (m_hp != 0) {
		if (!weapon[activeWeapon]->isReloading()) {
			Move();
			if (!isFlying()) {
				Shot();
			}
			Reload();
			changeWeapon(action.isWeaponLeft(), action.isWeaponRight());
		}
		mover.Update();
		m_model.SetPos(getPosition());
		m_model.SetRot(mover.getRotation());
		miniHpbar.setPos(getPosition());
		CVector3 pos = getPosition();
		pos.y += mover.GetCollider()->GetHeight()/2 + mover.GetCollider()->GetRadius();
		m_collision.SetPosition(pos);

		if (getPosition().y < -100.0f) {
			damage({ 0,0,0 }, maxHp / 5);
			mover.SetPosition(team->getHome());
		}
	} else {
		deathCool -= GetDeltaTimeSec();
		if (deathCool <= 0.0f) {
			Revive();
		}
	}
}

void CPlayer::sendAction(const ActionSender& actionPal) {
	action = actionPal;
}

bool CPlayer::damage(const CVector3 & dir, unsigned int damage, const Team* atkTeam, const CPlayer*) {
	if (m_hp != 0) {
		if (atkTeam == team) {
			damage = 0;
		}
		CVector3 pos = getPosition();
		pos.y += 60.0f;
		new GameObj::Suicider::CEffekseer(L"Resource/effect/damage.efk", 1.0f, pos);
		playSE(L"Resource/sound/SE_damage.wav", getPosition());
		mover.addVelocity(dir);
		if (m_hp > damage) {
			m_hp -= damage;
		} else {
			m_hp = 0;
			Death();
		}
		miniHpbar.display(m_hp);
		return true;
	}
	return false;
}

//死亡処理
void CPlayer::Death() {
	m_hp = 0;
	deathCool = constDeathCool;
	m_model.SetIsDraw(false);
	weapon[activeWeapon]->Inactivate();
}
//蘇生処理
void CPlayer::Revive() {
	m_hp = maxHp;
	m_model.SetIsDraw(true);
	weapon[activeWeapon]->Activate();
}

void CPlayer::Move() {

	const CVector2& movement = action.getMovement();

	//壁ジャンプ
	bool isWalljump = false;
	//空中に壁に当たりながらジャンプ
	if (mover.IsContactWall() && !mover.IsOnGround() && action.isJump()) {
		playSE(L"Resource/sound/SE_jump.wav", getPosition()); m_isSendJumpSE = true;
		mover.walljump(jumpPower, movement);
		isWalljump = true;
	}
	//飛行で壁に突っ込む
	if (mover.isHitWall()) {
		//mover.SetFlyTimer(max(0.0f, mover.getFlyTimer() - mover.getFlyTimerMax()*0.1f));//飛行可能時間を消費
		playSE(L"Resource/sound/SE_jump.wav", getPosition());
		isWalljump = true;
	}

	//ジャンプと飛行
	if (action.isJump() && !isWalljump) {
		if (mover.IsOnGround()) {
			playSE(L"Resource/sound/SE_jump.wav", getPosition()); m_isSendJumpSE = true;
			mover.jump(jumpPower);
		}
		else{
			if (mover.isFlying()) {
				mover.flyStop();
			} else {
				mover.fly(true, action.getLookVec(), {0, 0}, flyPower);
			}
		}
	}

	//飛行
	if (mover.isFlying()) {
		if (wing == nullptr) {
			wing = new Wing(mover);
		}
		mover.fly(false, action.getLookVec(), movement);
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
	if(action.isDash() && !action.isShot()) {
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

void CPlayer::changeWeapon(bool left, bool right) {
	if (left == right) { return; }
	int nextWeapon = (int)activeWeapon;
	if (left) { nextWeapon -= 1;}
	if (right) { nextWeapon += 1;}
	if (nextWeapon < 0) { nextWeapon = WEAPON_NUM - 1;}
	if (nextWeapon >= WEAPON_NUM) { nextWeapon = 0;}
	
	weapon[activeWeapon]->Inactivate();
	weapon[nextWeapon]->Activate();
	activeWeapon = (unsigned int)nextWeapon;
}
void CPlayer::changeWeapon(unsigned char useWeapon) {
	short nextWeapon = useWeapon;
	if (nextWeapon < 0) { nextWeapon = WEAPON_NUM - 1; }
	if (nextWeapon >= WEAPON_NUM) { nextWeapon = 0; }

	weapon[activeWeapon]->Inactivate();
	weapon[nextWeapon]->Activate();
	activeWeapon = nextWeapon;
}
