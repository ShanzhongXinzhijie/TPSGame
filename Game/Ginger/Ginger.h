#pragma once
#include "Wosiris.h"

class Ginger :
	public IGameObject
{
public:
	Ginger(int time);
	~Ginger();

	bool Start()override;
	void Update()override;

private:
	int m_isKensetued = false;
	int m_kensetuLeftTime = 0;

	CVector3 m_pos;
	CQuaternion m_rot;

	GameObj::CSkinModelRender m_model;
	PhysicsStaticObject m_phyStaticObject;
	SuicideObj::CCollisionObj m_collision[2];
};

