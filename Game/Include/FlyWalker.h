#pragma once
#include "Walker.h"
class FlyWalker : public Walker {
public:
	FlyWalker();
	~FlyWalker();

	void fly(bool isFly, const CVector3& velocity = {}, const CVector2& move = {}, float power = 0.0f);

	void flyStop();

	CQuaternion getRotation() const;

	float getFlyTimer() const{
		return flyTimer;
	}
	float getFlyTimerMax() const{
		return c_flyTimer;
	}

	bool isFlying()const {
		return flying;
	}

	bool isHitWall() const {
		return hitWall;
	}

	float GetFlyPower()const {
		return flyPower;
	}

	//通信受信で使う
	void SetIsFly(bool f) { flying = f; }
	void SetFlyPower(float p) { flyPower = p; }

	//飛行可能時間をセットする
	void SetFlyTimer(float timer) { flyTimer = timer; }

	void Update();

	static constexpr float c_flyTimer = 5.0f;
private:
	float flyTimer = c_flyTimer; //飛行可能な残り時間
	float upDown = 0.0f;

	bool hitWall = false;

	float flyPower = 0.0f;
	bool flying = false;

	SuicideObj::CSE* se = nullptr;
};

