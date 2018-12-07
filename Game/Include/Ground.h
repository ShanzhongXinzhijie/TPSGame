#pragma once
using namespace GameObj;

class Ground : public IGameObject{
public:
	Ground(CVector3 pos);
	
	void Update()override;
	bool Start()override;
private:
	GameObj::CSkinModelRender m_model;
	PhysicsStaticObject m_phyStaticObject;

	CVector3 m_pos;

	GameObj::CDirectionLight m_dirlight[2];
};

