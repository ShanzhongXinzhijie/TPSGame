#pragma once
#include "ICitizenBrain.h"
#include "CPlayer.h"
#include "Citizen.h"
class kansen : public ICitizenBrain
{
public:
	kansen(std::unordered_map<int, CPlayer*>& playersMap,const CVector3& citizen,Team*& team);
	~kansen();

	void Update(bool isOnGround)override;

	bool isAtk()override;

	CVector3 getMove()override;
	CQuaternion getTurn()override;

private:
	CVector3 walkVec = { 0, 0, 100 };
	CQuaternion rot;
	float jumpPower = 0;

	bool Atk;

	const std::unordered_map<int, CPlayer*>& playersMap;

	const CVector3& citizen_pos;

	Team*& team;
};

