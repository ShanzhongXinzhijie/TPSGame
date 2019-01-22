#pragma once
#include "Walker.h"
class FlyWalker : public Walker {
public:
	FlyWalker();
	~FlyWalker();

	void fly(bool isFly, const CVector3& velocity = {}, float power = 0.0f);

	CQuaternion getRotation();

	bool isFlying() {
		return flying;
	}

	void Update();

private:

	bool flying = false;
	bool fall = false;
};

