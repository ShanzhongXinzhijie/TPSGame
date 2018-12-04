#pragma once
#include "DemolisherWeapon/physics/CollisionAttr.h"

class CPlayer;

class BatBullet : public IGameObject {
	using CW = btCollisionWorld;

public:
	BatBullet(CPlayer* player, CVector3 position, CVector3 direction);
	~BatBullet();
	bool Start() override;
	void Update() override;

private:
	class Callback : public CW::ConvexResultCallback {
	public:
		bool isHit = false;

		explicit operator bool() const {
			return isHit;
		}

		btScalar addSingleResult(CW::LocalConvexResult& convexResult, bool normalInWorldSpace)override {
			if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Character
				&& convexResult.m_hitCollisionObject->getInternalType() != btCollisionObject::CO_GHOST_OBJECT) {
				isHit = true;
			}
			return 0.0f;
		}
	};

	CVector3 getHitVec() const;

	GameObj::CSkinModelRender m_model;
	CVector3 m_pos;
	CVector3 m_dir;
	float lifeTime = 5;
	CPlayer* const shotPlayer;

	SuicideObj::CCollisionObj m_collision;
};

