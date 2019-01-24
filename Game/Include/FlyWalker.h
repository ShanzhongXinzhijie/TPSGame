#pragma once
#include "Walker.h"
class FlyWalker : public Walker {
public:
	FlyWalker();
	~FlyWalker();

	void fly(bool isFly, const CVector3& velocity = {}, float power = 0.0f);

	void flyStop();

	CQuaternion getRotation();

	bool isFlying() {
		return flying;
	}

	void Update();

private:
	static constexpr float c_flyTimer = 10.0f;
	float flyTimer = c_flyTimer; //飛行可能な残り時間

	float flyPower = 0.0f;
	bool flying = false;
	static constexpr float flyGravity = gravity *100; //重力加速度
};

