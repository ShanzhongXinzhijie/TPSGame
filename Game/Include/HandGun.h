#pragma once
#include "Weapon.h"

class HandGun : public Weapon{
public:
	HandGun(CPlayer* player, GameObj::CSkinModelRender* playerModel,
			unsigned int shotAnimNum, unsigned int reloadAnimNum);
	~HandGun();

private:
	static WeaponInfo getInfo(unsigned int shotAnim, unsigned int reloadAnim);
};

