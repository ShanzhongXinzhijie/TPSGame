#pragma once

class Ground : public IGameObject{
public:
	Ground(CVector3 pos);
	
	void Update()override;
	bool Start()override;
private:
	GameObj::CSkinModelRender m_skyModel;

	GameObj::CSkinModelRender m_model;
	PhysicsStaticObject m_phyStaticObject;

	CVector3 m_pos;

	const CVector3* cameraPos;

	GameObj::CDirectionLight m_dirlight;
	ShadowMapHandler shadowMap;
};

