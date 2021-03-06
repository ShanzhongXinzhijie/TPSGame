#include "stdafx.h"
#include "sikakukuidou.h"


sikakukuidou::sikakukuidou()
{
}


sikakukuidou::~sikakukuidou()
{
}

void sikakukuidou::Update(bool isOnGround, float deltaTime)
{
	if (!isOnGround) {
		jumpPower -= 980.0f * deltaTime;
	}
	else {
		if (jumpPower < 0) {
			jumpPower = 0;
		}
		sec -= deltaTime;
		if (sec <= 0.0f) {
			rot.Multiply(CQuaternion::GetRotationDeg(CVector3::AxisY(), 45 * 2));
			sec = 4.0f;
		}
	}
}

CVector3 sikakukuidou::getMove()
{
	CVector3 walk = walkVec;
	rot.Multiply(walk);
	walk.y = jumpPower;
	return walk;
}

CQuaternion sikakukuidou::getTurn()
{
	return rot;
}
