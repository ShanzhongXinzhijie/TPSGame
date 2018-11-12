#pragma once
using namespace GameObj;

class Ground : public IGameObject{
public:
	bool Start()override;
private:
	GameObj::CSkinModelRender m_model;
	PhysicsStaticObject m_phyStaticObject;

	GameObj::CDirectionLight m_dirlight[2];
};

