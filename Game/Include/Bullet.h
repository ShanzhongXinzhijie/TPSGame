#pragma once
#include "DemolisherWeapon/physics/CollisionAttr.h"

class CPlayer;

class Bullet : public IGameObject {
	using CW = btCollisionWorld;

public:
	Bullet(CPlayer* player, CVector3 position, CVector3 direction, const wchar_t* modelPath);
	~Bullet();
	void Update() override;

	CVector3 getHitVec() const;
	CPlayer* getShooter() {
		return shotPlayer;
	};

	void addVelocity(const CVector3& velocity) {
		m_dir += velocity;
	}

private:
	GameObj::CSkinModelRender m_model;
	CVector3 m_pos;
	CVector3 m_dir;
	float lifeTime = 5;
	CPlayer* const shotPlayer;

	SuicideObj::CCollisionObj m_collision;
};

