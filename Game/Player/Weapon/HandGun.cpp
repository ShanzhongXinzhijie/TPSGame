#include "stdafx.h"
#include "HandGun.h"
#include "NormalBullet.h"

HandGun::HandGun(CPlayer* player, GameObj::CSkinModelRender* playerModel, unsigned int shotAnim,
				 unsigned int reloadAnim)
	:Weapon(player, playerModel, getInfo(shotAnim, reloadAnim))
{
}


HandGun::~HandGun() {
}

Bullet* HandGun::createBullet(CPlayer * player, CVector3 pos, CVector3 dir) {
	return new NormalBullet(player, pos, dir*3000.0f, L"Resource/modelData/Bat.cmo", 84);
}

WeaponInfo HandGun::getInfo(unsigned int shotAnim, unsigned int reloadAnim) {
	WeaponInfo info;
	info.modelPath = L"Resource/modelData/HundGun.cmo";
	info.maxBullet = 50;
	info.shotCool = 0.2f;
	info.zoomScale = 1.0f;
	info.shotAnimNum = shotAnim;
	info.reloadAnimNum = reloadAnim;
	return info;
}
