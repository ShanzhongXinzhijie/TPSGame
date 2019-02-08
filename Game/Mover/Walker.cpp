#include "stdafx.h"
#include "Walker.h"


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
		playSE(L"Resource/sound/SE_fall.wav");
	}
	beforeGround = IsOnGround();

	//位置更新
	Execute(velocity);

	//SE
	if (IsOnGround() && velocity.LengthSq() > 10.0f*10.0f) {
		footTime += GetDeltaTimeSec() * velocity.Length();
		if (footTime > c_footTime) {
			playSE(L"Resource/sound/SE_foot.wav");
			footTime = 0.0f;
		}
	}
}

void Walker::turn(float x, float z) {
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
		}
	}
}

void Walker::addVelocity(const CVector3& v) {
	velocity += v;
}

void Walker::playSE(const wchar_t * path) {
	SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(path);
	se->SetPos(GetPosition());//音の位置
	se->SetDistance(500.0f);//音が聞こえる範囲
	se->Play(true); //第一引数をtrue
}


