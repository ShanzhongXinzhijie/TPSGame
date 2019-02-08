#pragma once
#include "DemolisherWeapon/physics/character/CCharacterController.h"

class Walker : public CCharacterController{
public:
	Walker();
	~Walker();

	void move(CVector2 move);
	void jump(float jumpPower);
	void walljump(float jumpPower, CVector2 move);
	void Update();

	void turn(float x, float z);

	void addVelocity(const CVector3& velocity);

	CVector3 getVelocity()  const{
		return velocity;
	}

	CQuaternion getRotation() const{
		return m_rot;
	}

protected:
	void playSE(const wchar_t* path);

	CVector3 velocity = { 0.0f, 0.0f, 0.0f };	 //ë¨ìx

	CQuaternion m_rot;
	float radian = 0.0f; //âÒì]ó 

	bool beforeGround = true;

	static constexpr float friction = 10.0f; //ñÄéC
	static constexpr float gravity = 980.0f; //èdóÕâ¡ë¨ìx

	SuicideObj::CSE* footSe = nullptr;
	static constexpr float c_footTime = 200.0f;
	float footTime = 0.0f;
};

