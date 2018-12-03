#pragma once

class ICitizenBrain {
public:
	ICitizenBrain();
	~ICitizenBrain();

	virtual void Update(bool isOnGround) = 0;

	virtual CVector3 getMove() = 0;
	virtual CQuaternion getTurn() = 0;

	virtual bool isAtk();
};

