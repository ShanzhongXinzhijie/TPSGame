#pragma once

class ICitizenMover {
public:
	ICitizenMover();
	~ICitizenMover();

	virtual void Update(bool isOnGround) = 0;

	virtual CVector3 getMove() = 0;
	virtual CQuaternion getTurn() = 0;
};

