#pragma once
#include "Wosiris.h"

class Ginger :
	public IGameObject
{
public:
	Ginger(int timeSec);
	~Ginger();

	bool Start()override;
	void Update()override;

private:
	void Konryu();

	int m_isKensetued = false;
	int m_kensetuLeftTime = 0, m_kensetuLeftTimeMax = 0;
	float m_uvScroll = 0.0f;
	
	int m_hp = 3000;

	CVector3 m_pos;
	CQuaternion m_rot;

	GameObj::CSkinModelRender m_model;
	PhysicsStaticObject m_phyStaticObject;
	SuicideObj::CCollisionObj m_collision[2];
};

