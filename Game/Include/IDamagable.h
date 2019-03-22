#pragma once
#include "DemolisherWeapon/math/Vector.h"
#include "Team.h"

class IDamagable {
public:
	virtual ~IDamagable() = default;

	virtual bool damage(const DemolisherWeapon::CVector3& vec ,unsigned int damage,
						const Team* team = nullptr, const CPlayer* player = nullptr) = 0;
};

