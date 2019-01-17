#include "stdafx.h"
#include "ittarikitari.h"


ittarikitari::ittarikitari()
{
}


ittarikitari::~ittarikitari()
{
}

void ittarikitari::Update(bool isOnGround, float deltaTime)
{
	if (!isOnGround) {
		jumpPower -= 980.0f * deltaTime;
	}
	sec -= deltaTime;
	if (sec <= 0.0f) {
		rot.Multiply(CQuaternion::GetRotationDeg(CVector3::AxisY(), 180 * 1));
		sec = 4.0f;
	}
}

CVector3 ittarikitari::getMove()
{
	CVector3 walk = walkVec;
	rot.Multiply(walk);
	walk.y = jumpPower;
	return walk;
}

CQuaternion ittarikitari::getTurn()
{
	return rot;
}
