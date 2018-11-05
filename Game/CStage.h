#pragma once

#include "DemolisherWeapon/physics/PhysicsStaticObject.h"
#include "DemolisherWeapon/physics/CapsuleCollider.h"
#include "DemolisherWeapon/physics/RigidBody.h"

class CStage : public IGameObject {
public:
	bool Start()override;
private:
	GameObj::CSkinModelRender m_model, m_sky;
	PhysicsStaticObject m_phyStaticObject;

	GameObj::CDirectionLight m_dirlight[4];
	std::vector<GameObj::CPointLight> m_poilight;
};

