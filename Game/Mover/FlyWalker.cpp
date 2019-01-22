#include "stdafx.h"
#include "FlyWalker.h"


FlyWalker::FlyWalker() {
}


FlyWalker::~FlyWalker() {
}

void FlyWalker::fly(bool isFly, const CVector3 & v, float power) {
	if (isFly) {
		if(!fall){
			CVector3 flyVelocity = {};
			flyVelocity += v;
			flyVelocity.Normalize();
			flyVelocity.y += 0.2;
			flyVelocity *= power;
			addVelocity(flyVelocity);
			flying = true;
		}
	} else {
		fall = false;
	}
}

CQuaternion FlyWalker::getRotation() {
	if (flying && !fall) {
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
	if (flying && !fall) {
		if (IsOnGround()) {
			flying = false;
			fall = true;
		}
	}
	Walker::Update();
}
