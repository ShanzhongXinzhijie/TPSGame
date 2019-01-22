#pragma once
#include "DemolisherWeapon/physics/character/CCharacterController.h"

class Walker : public CCharacterController{
public:
	Walker();
	~Walker();

	void move(CVector2 move);
	void jump(float jumpPower);
	void Update();

	void turn(float x, float z);

	void addVelocity(const CVector3& velocity);

	CQuaternion getRotation() {
		return m_rot;
	}

protected:
	CVector3 velocity = { 0.0f, 0.0f, 0.0f };	 //���x

	CQuaternion m_rot;
	float radian = 0.0f; //��]��

	static constexpr float friction = 10.0f; //���C
	static constexpr float gravity = 980.0f; //�d�͉����x
};

