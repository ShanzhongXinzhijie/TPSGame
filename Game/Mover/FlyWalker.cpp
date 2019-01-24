#include "stdafx.h"
#include "FlyWalker.h"


FlyWalker::FlyWalker() {
}


FlyWalker::~FlyWalker() {
}

void FlyWalker::fly(bool isFly ,const CVector3 & v, float power) {
	if (isFly) {
		if (flyTimer > c_flyTimer*0.5) {
			flyPower = power;
			velocity = v * flyPower;
			flying = true;
		}
	} else if(flying) {
		velocity = v * flyPower * GetDeltaTimeSec();
		flyPower += -v.y * flyGravity * GetDeltaTimeSec();
		if (flyPower <= 0) {
			flyStop();
		}
	}
}

void FlyWalker::flyStop() {
	velocity *= 0.5f;
	flying = false;
}

CQuaternion FlyWalker::getRotation() {
	if (flying) {
		CQuaternion rot;
		rot.SetRotation(CVector3::AxisY(), atan2f(velocity.x, velocity.z));

		float xz = velocity.x*velocity.x + velocity.z*velocity.z;
		xz = sqrt(xz);

		rot.Multiply(CQuaternion::GetRotation(CVector3::AxisX(), atan2f(-velocity.y, xz)-CMath::PI * -0.5));

		return rot;
	} else {
		return Walker::getRotation();
	}
}

void FlyWalker::Update() {
	if (flying) {
		flyTimer -= GetDeltaTimeSec();
		if (flyTimer < 0.0f) {
			flyTimer = 0.0f;
		}

		if (IsOnGround() || flyTimer == 0.0f) {
			flyStop();
		}
	} else {
		if (flyTimer < c_flyTimer) {
			flyTimer += GetDeltaTimeSec() * 0.5f;
			if (flyTimer > c_flyTimer) {
				flyTimer = c_flyTimer;
			}
		}
	}
	Walker::Update();
}
