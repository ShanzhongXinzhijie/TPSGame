#pragma once
#include "Weapon.h"
class Rifle :
	public Weapon {
public:
	Rifle(CPlayer* player, GameObj::CSkinModelRender* playerModel,
		  unsigned int shotAnimNum, unsigned int reloadAnimNum);
	~Rifle();

private:
	Bullet* createBullet(CPlayer* player, CVector3 pos, CVector3 dir);
	static WeaponInfo getInfo(unsigned int shotAnim, unsigned int reloadAnim);
};

