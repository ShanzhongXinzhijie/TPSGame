#include "stdafx.h"
#include "Rifle.h"
#include "NormalBullet.h"

Rifle::Rifle(CPlayer* player, GameObj::CSkinModelRender* playerModel,
			 unsigned int shotAnim, unsigned int reloadAnim)
	:Weapon(player, playerModel, getInfo(shotAnim, reloadAnim)){
}


Rifle::~Rifle() {
}

Bullet* Rifle::createBullet(CPlayer * player, CVector3 pos, CVector3 dir) {
	return new NormalBullet(player, pos, dir*5000.0f, L"Resource/modelData/RifleBullet.cmo",250);
}

WeaponInfo Rifle::getInfo(unsigned int shotAnim, unsigned int reloadAnim) {
	WeaponInfo info;
	info.modelPath = L"Resource/modelData/Rifle.cmo";
	info.maxBullet = 20;
	info.shotCool = 2.0f;
	info.zoomScale = 5.0f;
	info.shotAnimNum = shotAnim;
	info.reloadAnimNum = reloadAnim;
	return info;
}
