#include "stdafx.h"
#include "CPlayer.h"
#include "BatBullet.h"

CPlayer::CPlayer(int pNum,CVector4 col, const CVector3& position)
	: playerNum(pNum),m_pos(position), color(col){
}

CPlayer::~CPlayer() {
}

bool CPlayer::Start() {
	m_animationClips[anim_run].Load(L"Resource/animData/PlayerRun.tka", true);
	m_animationClips[anim_walk].Load(L"Resource/animData/PlayerWalk.tka", true);
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

	m_model.GetSkinModel().FindMaterial([&](ModelEffect* mat) {
		mat->SetAlbedoScale(color);
	});

	charaCon.Init(30.0f, 90.0f, m_pos);

	m_collision.CreateCapsule(m_pos, CQuaternion::Identity(), 30.0f, 80.0f);
	m_collision.SetName(L"CPlayer");
	m_collision.SetClass(this);

	return true;
};

void CPlayer::Update() {
	if (m_hp != 0) {
		if (!onReload) {
			GravityAndJump();
			Move();
			Turn();
			Shot();
			Reload();
		}
	} else {
		deathCool -= GetDeltaTimeSec();
		if (deathCool <= 0.0f) {
			m_hp = constHp;
			m_model.SetIsDraw(true);
		}
	}
}

CVector3 CPlayer::getPosition() {
	return m_pos;
}

void CPlayer::sendAction(const ActionSender& actionPal) {
	action = actionPal;
}

bool CPlayer::BatHit(CPlayer* player, CVector3 dir) {
	if (this != player && m_hp != 0) {
		velocity += dir;
		m_hp--;
		if (m_hp == 0) {
			deathCool = constDeathCool;
			m_model.SetIsDraw(false);
		}
		return true;
	}
	return false;
}

void CPlayer::GravityAndJump() {
	if (!charaCon.IsOnGround()) {
		velocity.y -= gravity * GetDeltaTimeSec();
	} else {
		if (velocity.y < 0) {
			velocity.y = 0;
		}
		if (action.isJump()) {
			velocity.y = jumpPower;
		}
	}
}

void CPlayer::Move() {
	const CVector2& movement = action.getMovement();

	//ダッシュ判定
	float speed = moveSpeed;
	bool dash = false;
	if (action.isDash()) {
		speed = moveSpeed * dashMul;
		dash = true;
	}

	//射撃中は移動速度減少
	if (action.isShot()) {
		speed = moveSpeed * 0.5f;
	}

	//空中にいる場合空気抵抗減少、操作は効きにくくなる。(摩擦は空気抵抗として計算する)
	float airResist = friction;
	if (!charaCon.IsOnGround() || action.isJump()) {
		speed = moveSpeed * 0.1f;
		airResist *= 0.1f;
	}

	//x移動
	velocity.x += movement.x * speed;

	//z移動
	velocity.z += movement.y * speed;

	//空気抵抗。
	{
		char sign = 1;
		if (velocity.x < 0) {
			sign = -1;
		}
		velocity.x -= airResist * velocity.x * GetDeltaTimeSec();
		if (velocity.x * sign < 0) {
			velocity.x = 0;
		}

		sign = 1;
		if (velocity.z < 0) {
			sign = -1;
		}
		velocity.z -= airResist * velocity.z * GetDeltaTimeSec();
		if (velocity.z * sign < 0) {
			velocity.z = 0;
		}
	}

	//位置更新
	m_pos = charaCon.Execute(velocity);

	//アニメーションと回転
	if (movement.x != 0 || movement.y != 0) {
		if (dash && !action.isShot()) {
			m_model.GetAnimCon().Play(anim_run, animInterpolateSec);
		} else {
			m_model.GetAnimCon().Play(anim_walk, animInterpolateSec);
		}
	} else {
		m_model.GetAnimCon().Play(anim_idle, animInterpolateSec);
	}

	//モデル更新
	m_model.SetPos(m_pos);
	//コリジョン更新
	m_collision.SetPosition(m_pos);
}

void CPlayer::Turn() 	{
	float x;
	float z;
	if (action.isShot()) {
		x = action.getLookVec().x;
		z = action.getLookVec().z;
	} else {
		x = action.getMovement().x;
		z = action.getMovement().y;
	}

	if (z != 0 || x != 0) {
		float rotRad = atan2f(x, z) - radian;
		if (rotRad != 0) {
			char sign = 1;
			if (rotRad < 0) {
				sign = -1;
			}
			if (CMath::PI < rotRad*sign) {
				rotRad -= CMath::PI2*sign;
				sign = -sign;
			}
			float delta = GetDeltaTimeSec();
			if (rotRad*sign < CMath::PI * delta * 7) {
				radian += rotRad;
			} else {
				radian += sign * CMath::PI * delta * 7;
			}
			if (radian < -CMath::PI) {
				radian += CMath::PI2;
			} else if (CMath::PI < radian) {
				radian -= CMath::PI2;
			}
			m_rot.SetRotation(CVector3::AxisY(), radian);
			m_model.SetRot(m_rot);
		}
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
		CVector3 look = action.getLookVec();
		CVector3 pos = m_pos;
		pos.y += 60;
		new BatBullet(this, pos, look * 30);
		bulletCount--;
	}
}

void CPlayer::Reload() {
	if (action.isReload() && bulletCount < constBulletCount) {
		m_model.GetAnimCon().Play(anim_reload, 0.5f);
		onReload = true;
	}
}
