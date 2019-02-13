#include "stdafx.h"
#include "FlyWalker.h"


FlyWalker::FlyWalker() {
}


FlyWalker::~FlyWalker() {
	if (se != nullptr) {
		se->Stop();
	}
}

void FlyWalker::fly(bool isFly ,const CVector3 & v, float power) {
	if (isFly) {
		if (flyTimer > 0.0f) {
			playSE(L"Resource/sound/SE_fly.wav");
			se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_flying.wav");
			se->SetPos(GetPosition());
			se->SetDistance(500.0f);
			se->Play(true, true);
			flyPower = power;
			velocity = v * flyPower;
			flying = true;
		}
	} else if(flying) {
		if (se != nullptr) {
			se->SetPos(GetPosition());
		}

		velocity = v * flyPower * GetDeltaTimeSec();
		if (flyPower <= 0) {
			flyStop();
		}
	}
}

void FlyWalker::flyStop() {
	velocity *= 0.5f;
	flying = false;
	se->Stop();
	se = nullptr;
}

CQuaternion FlyWalker::getRotation() {
	if (flying) {
		CQuaternion rot;
		rot.SetRotation(CVector3::AxisY(), atan2f(velocity.x, velocity.z));

		float xz = velocity.x*velocity.x + velocity.z*velocity.z;
		xz = sqrt(xz);

		rot.Multiply(CQuaternion::GetRotation(CVector3::AxisX(), atan2f(-velocity.y, xz)-(CMath::PI * -0.5)));

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
		if (IsOnGround() && flyTimer < c_flyTimer) {
			flyTimer += GetDeltaTimeSec();
			if (flyTimer > c_flyTimer) {
				flyTimer = c_flyTimer;
			}
		}
	}
	Walker::Update();
}
