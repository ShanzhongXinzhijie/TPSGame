#pragma once
#include "ICitizenBrain.h"
#include "CPlayer.h"
class Citizennigeru : public ICitizenBrain{
public:
	Citizennigeru(const std::unordered_map<int, CPlayer*>& playersMap);
	~Citizennigeru();

	void Update(bool isOnGround, float deltaTime) override;

	CVector3 getMove() override;
	CQuaternion getTurn() override;

	//í êMóp
	CVector3 GetNetVec()const override {
		return walk;
	}
	void SetNetVec(const CVector3& v) override {
		walk = v;
	}
private:

	const std::unordered_map<int, CPlayer*>& playersMap;

	CVector3 walkVec = { 0, 0, 100 };
	CQuaternion rot;
	float jumpPower = 0;

	CVector3 walk;
};

