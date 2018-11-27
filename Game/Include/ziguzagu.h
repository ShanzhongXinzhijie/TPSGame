#pragma once
#include "ICitizenMover.h"
class ziguzagu :	public ICitizenBrain{
public:
	ziguzagu();
	~ziguzagu();

	void Update(bool isOnGround) override;

	CVector3 getMove() override;
	CQuaternion getTurn() override;

private:
	float jumpPower = 0;
	CVector3 walkVec = { 0, 0, 100 };
	CQuaternion rot;

	float sec = 2.0f;
	int kirikae = 0;
	int hanten = 0;
	int hantenCount = 0;
};

