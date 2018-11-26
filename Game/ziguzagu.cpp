#include "stdafx.h"
#include "Include\ziguzagu.h"


ziguzagu::ziguzagu()
{
}


ziguzagu::~ziguzagu()
{
}

void ziguzagu::Update(bool isOnGround)
{
	if (!isOnGround) {
		jumpPower -= 980.0f * GetDeltaTimeSec();
	}
	sec -= GetDeltaTimeSec();
	if (sec <= 0.0f) {
		if (hanten == 0) {
			if (kirikae == 0) {
				rot.Multiply(CQuaternion::GetRotationDeg(CVector3::AxisY(), 45 * 1));
				kirikae = 1;
			}
			else if (kirikae == 1) {
				rot.Multiply(CQuaternion::GetRotationDeg(CVector3::AxisY(), 45 * -1));
				kirikae = 0;
			}
			hantenCount++;
			if (hantenCount == 4) {
				hanten = 1;
				hantenCount = 0;
				rot.Multiply(CQuaternion::GetRotationDeg(CVector3::AxisY(), 225 * 1));
			}
		}
		else if (hanten == 1) {
			if (kirikae == 0) {
				rot.Multiply(CQuaternion::GetRotationDeg(CVector3::AxisY(), -45 * 1));
				kirikae = 1;
			}
			else if (kirikae == 1) {
				rot.Multiply(CQuaternion::GetRotationDeg(CVector3::AxisY(), -45 * -1));
				kirikae = 0;
			}
			hantenCount++;
			if (hantenCount == 4) {
				hanten = 0;
				hantenCount = 0;
				rot.Multiply(CQuaternion::GetRotationDeg(CVector3::AxisY(), 135 * 1));
			}
		}
		sec = 2.0f;
	}
}

CVector3 ziguzagu::getMove()
{
	CVector3 walk = walkVec;
	rot.Multiply(walk);
	walk.y = jumpPower;
	return walk;
}

CQuaternion ziguzagu::getTurn()
{
	return rot;
}
