#pragma once
#include "Citizen.h"
class ICitizenBrain {
public:
	ICitizenBrain();
	~ICitizenBrain();

	virtual void Update(bool isOnGround, float deltaTime) = 0;

	virtual CVector3 getMove() = 0;
	virtual CQuaternion getTurn() = 0;

	virtual bool isAtk();

	void SetCitizen(Citizen* setCitizen);

	Citizen* citizen;
};

