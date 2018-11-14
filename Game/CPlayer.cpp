#include "stdafx.h"
#include "CPlayer.h"
#include "BatBullet.h"

CPlayer::CPlayer() {
}

bool CPlayer::Start() {
	m_animationClips[anim_run].Load(L"Resource/animData/run.tka", true);
	m_animationClips[anim_walk].Load(L"Resource/animData/walk.tka", true);
	m_animationClips[anim_idle].Load(L"Resource/animData/idle.tka", true);

	m_model.Init(L"Resource/modelData/unityChan.cmo", m_animationClips, anim_num, enFbxUpAxisY);

	charaCon.Init(30.0f, 80.0f, m_pos);

	return true;
};

void CPlayer::Update() {
	GravityAndJump();
	Move();
	Turn();
	Shot();
	//アップデート終了時、アクションを初期化する。
	action = ActionSender();
}

CVector3 CPlayer::getPosition() {
	return m_pos;
}

void CPlayer::sendAction(const ActionSender& actionPal) {
	action = actionPal;
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
	if (shotCoolTime <= 0) {
		if (action.isShot()) {
			shot = true;
			shotCoolTime = coolTime;
		}
	} else {
		shotCoolTime -= GetDeltaTimeSec();
	}

	if (shot) {
		CVector3 look = action.getLookVec();
		CVector3 pos = m_pos;
		pos.y += 70;
		new BatBullet(pos, look * 30);
	}
}