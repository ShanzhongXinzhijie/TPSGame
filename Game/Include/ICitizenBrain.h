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

	//�ʐM�p
	virtual CVector3 GetNetVec() const{
		return { -1.0f,-1.0f,-1.0f };
	}
	virtual void SetNetVec(const CVector3& v) {
		v;
	}
};

