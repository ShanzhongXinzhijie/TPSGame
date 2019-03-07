#pragma once
#include "DemolisherWeapon/physics/CollisionAttr.h"
#include "Bullet.h"

class CPlayer;

class NormalBullet : public Bullet{
public:
	NormalBullet(CPlayer* player, CVector3 position, CVector3 direction, const wchar_t* modelPath);
	~NormalBullet();
};

