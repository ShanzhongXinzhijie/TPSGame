#pragma once
#include "ICitizenBrain.h"
class sikakukuidou :	public ICitizenBrain{
public:
	sikakukuidou();
	~sikakukuidou();

	void Update(bool isOnGround, float deltaTime)override;
	
	CVector3 getMove()override;
	CQuaternion getTurn()override;

private:
	CVector3 walkVec = { 0, 0, 100 };
	CQuaternion rot;
	float sec = 4.0f;
	float jumpPower = 0;
};

