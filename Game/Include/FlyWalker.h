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

	//��s�\���Ԃ��Z�b�g����
	void SetFlyTimer(float timer) { flyTimer = timer; }

	void Update();

	static constexpr float c_flyTimer = 10.0f;
private:
	float flyTimer = c_flyTimer; //��s�\�Ȏc�莞��

	float flyPower = 0.0f;
	bool flying = false;
	static constexpr float flyGravity = gravity *100; //�d�͉����x
};

