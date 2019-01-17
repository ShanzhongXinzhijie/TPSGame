#include "stdafx.h"
#include "CircleWalk.h"


CircleWalk::CircleWalk() {
}


CircleWalk::~CircleWalk() {
}

void CircleWalk::Update(bool isOnGround, float deltaTime) {
	rot.Multiply(CQuaternion::GetRotationDeg(CVector3::AxisY(), 45 * deltaTime));
	if (!isOnGround) {
		jumpPower -= 980.0f * deltaTime;
	} else {
		if (jumpPower < 0) {
			jumpPower = 0;
		}
		sec -= deltaTime;
		if (sec < 0){
			if (rand() % 4 == 0) {
				jumpPower = 600.0f;
			}
			sec = 1.0f;
		}
	}
}

CVector3 CircleWalk::getMove() {
	CVector3 walk = walkVec;
	rot.Multiply(walk);
	walk.y = jumpPower;
	return walk;
}

CQuaternion CircleWalk::getTurn() {
	return rot;
}
