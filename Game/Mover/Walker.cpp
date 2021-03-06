#include "stdafx.h"
#include "Walker.h"
#include "SE_Util.h"

Walker::Walker() {
}


Walker::~Walker() {
}

void Walker::move(CVector2 move) {
	if (!IsOnGround() || velocity.y > 0) {
		move *= 0.1f;
	}
	velocity.x += move.x;
	velocity.z += move.y;
}

void Walker::jump(float jumpPower) {
	if (IsOnGround()) {
		velocity.y = jumpPower;
	}	
}

void Walker::walljump(float jumpPower, CVector2 move) {
	if (IsContactWall()) {	
		CVector3 dir = GetContactWallNormal();
		dir.y = 0.0f;
		dir += CVector3(move.x, 0.0f, move.y)*0.5f;
		dir += CVector3::AxisY();
		velocity = dir * jumpPower;
	}
}

void Walker::Update() {
	springRotation();
	//重力
	if (!IsOnGround()) {
		velocity.y -= gravity * GetDeltaTimeSec();
	} else {
		if (velocity.y < 0) {
			velocity.y = 0;
		}
	}

	//空中にいる場合空気抵抗減少
	float airResist = friction;
	if (!IsOnGround() || velocity.y > 0) {
		airResist *= 0.1f;
	}

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

	if (!beforeGround && IsOnGround()) {
		playSE(L"Resource/sound/SE_fall.wav", GetPosition());
	}
	beforeGround = IsOnGround();

	//位置更新
	Execute(velocity);

	//SE
	if (IsOnGround() && velocity.LengthSq() > 10.0f*10.0f) {
		footTime += GetDeltaTimeSec() * velocity.Length();
		if (footTime > c_footTime) {
			playSE(L"Resource/sound/SE_foot.wav", GetPosition());
			footTime = 0.0f;
		}
	}
}

void Walker::turn(float x, float z) {
	if (z != 0 || x != 0) {
		m_rot.SetRotation(CVector3::AxisY(), atan2f(x, z));
	}
}

void Walker::addVelocity(const CVector3& v) {
	velocity += v;
}

void Walker::springRotation() {
	springRot.Slerp(0.1f, springRot, m_rot);
}


