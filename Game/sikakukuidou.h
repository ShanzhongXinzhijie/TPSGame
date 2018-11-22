#pragma once
#include "ICitizenMover.h"
class sikakukuidou :
	public ICitizenMover
{
public:
	sikakukuidou();
	~sikakukuidou();
	void Update(bool isOnGround);
	CVector3 getMove();
	CQuaternion getTurn();
};

