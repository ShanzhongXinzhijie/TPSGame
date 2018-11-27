#pragma once
#include "ICitizenMover.h"
class ittarikitari :	public ICitizenBrain{
public:
	ittarikitari();
	~ittarikitari();

	void Update(bool isOnGround) override;

	CVector3 getMove() override;
	CQuaternion getTurn() override;

private:
	float jumpPower = 0;
	CVector3 walkVec = { 0, 0, 100 };
	CQuaternion rot;

	float sec = 4.0f;
};

