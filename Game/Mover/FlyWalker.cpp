#include "stdafx.h"
#include "FlyWalker.h"


FlyWalker::FlyWalker() {
}


FlyWalker::~FlyWalker() {
	if (se != nullptr) {
		se->Stop();
	}
}

void FlyWalker::fly(bool isFly ,const CVector3 & v,const CVector2& move ,float power) {
	if (isFly) {
		if (flyTimer > 0.0f) {
			playSE(L"Resource/sound/SE_fly.wav");
			se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_flying.wav");
			se->SetPos(GetPosition());
			se->SetDistance(500.0f);
			se->Play(true, true);
			flyPower = power;
			CVector3 v2 = v;
			v2.y = upDown = 0;
			velocity = v2 * flyPower;
			velocity.y += gravity * GetDeltaTimeSec();
			flying = true;
		}
	} else if (flying) {
		if (se != nullptr) {
			se->SetPos(GetPosition());
		}
		CVector3 v2 = v;
		v2.y = 0;
		v2.Normalize();

		upDown += (move.y*2.4f - upDown) * GetDeltaTimeSec() * 2;

		v2.y = upDown;
		v2.Normalize();
		velocity = v2 * flyPower * GetDeltaTimeSec();
		velocity.y += gravity * GetDeltaTimeSec();

		if (flyPower <= 0) {
			flyStop();
		}
	}
}

void FlyWalker::flyStop() {
	velocity *= 0.5f;
	flying = false;
	if (se) {
		se->Stop();
		se = nullptr;
	}
}

CQuaternion FlyWalker::getRotation() const{
	if (flying) {
		CQuaternion rot;
		rot.SetRotation(CVector3::AxisY(), atan2f(velocity.x, velocity.z));

		float xz = velocity.x*velocity.x + velocity.z*velocity.z;
		xz = sqrt(xz);

		rot.Multiply(CQuaternion::GetRotation(CVector3::AxisX(), atan2f(-velocity.y, xz)+(CMath::PI * 0.5f)));

		return rot;
	} else {
		return Walker::getRotation();
	}
}

void FlyWalker::Update() {
	hitWall = false;
	if (flying) {
		flyTimer -= GetDeltaTimeSec();
		if (flyTimer < 0.0f) {
			flyTimer = 0.0f;
		}

		if (IsOnGround() || flyTimer == 0.0f) {
			flyStop();
		}

		//•Ç‚ÉÕ“Ë‚µ‚½ê‡
		btVector3 from = GetPosition();
		CVector3 vec = velocity;
		vec.Normalize();
		btVector3 to = from + vec * 100.0f;
		btCollisionWorld::ClosestRayResultCallback callback(from, to);
		callback.m_collisionFilterMask = 2;
		GetPhysicsWorld().RayTest(from, to, callback);
		if (callback.hasHit()) {
			flyStop();
			btVector3 normal = callback.m_hitNormalWorld;
			float x = sqrt(normal.getX()*normal.getX() + normal.getZ()*normal.getZ());
			float tan = fabsf(normal.getY() / x+0.00001f);
			if (tan < 0.5f) {
				hitWall = true;
				CVector3 dir;
				dir.Set(normal);
				dir.y = 0.0f;
				dir += CVector3::AxisY();
				velocity = dir * 600.0f;
			}
		}

	} else {
		if (IsOnGround() && flyTimer < c_flyTimer) {
			flyTimer += GetDeltaTimeSec();
			if (flyTimer > c_flyTimer) {
				flyTimer = c_flyTimer;
			}
		}
	}
	Walker::Update();
}
