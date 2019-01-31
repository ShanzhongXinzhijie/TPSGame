#pragma once
#include "Walker.h"
class FlyWalker : public Walker {
public:
	FlyWalker();
	~FlyWalker();

	void fly(bool isFly, const CVector3& velocity = {}, float power = 0.0f);

	void flyStop();

	CQuaternion getRotation();

	float getFlyTimer() {
		return flyTimer;
	}
	float getFlyTimerMax() const{
		return c_flyTimer;
	}

	bool isFlying() {
		return flying;
	}

	//飛行可能時間をセットする
	void SetFlyTimer(float timer) { flyTimer = timer; }

	void Update();

	static constexpr float c_flyTimer = 10.0f;
private:
	float flyTimer = c_flyTimer; //飛行可能な残り時間

	float flyPower = 0.0f;
	bool flying = false;
	static constexpr float flyGravity = gravity *100; //重力加速度
};

