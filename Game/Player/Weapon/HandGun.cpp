#include "stdafx.h"
#include "HandGun.h"
#include "CPlayer.h"
#include "BatBullet.h"

HandGun::HandGun(CPlayer* player, GameObj::CSkinModelRender* playerModel, unsigned int shotAnim,
				 unsigned int reloadAnim)
	:Weapon(player, playerModel, getInfo(shotAnim, reloadAnim))
{
}


HandGun::~HandGun() {
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
