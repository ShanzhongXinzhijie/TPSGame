#pragma once
#include "Wosiris.h"

enum GodPowerType {
	enWosiris,
	//enIndra,
	//enHades,
	//enOdin,
	enNum,
	enNone,
};

class Ginger :
	public IGameObject
{
public:
	Ginger(int timeSec, GodPowerType powerType);
	~Ginger();

	bool Start()override;
	void Update()override;

private:
	void Konryu();

	int m_isKensetued = false;
	int m_kensetuLeftTime = 0, m_kensetuLeftTimeMax = 0;
	float m_uvScroll = 0.0f;
	
	int m_hp = 3000;
	GodPowerType m_powerType = enNone;

	CVector3 m_pos;
	CQuaternion m_rot;

	GameObj::CSkinModelRender m_model;
	PhysicsStaticObject m_phyStaticObject;
	SuicideObj::CCollisionObj m_collision[2];
};

