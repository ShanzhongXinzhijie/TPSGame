#include "stdafx.h"
#include "Rifle.h"
#include "NormalBullet.h"

Rifle::Rifle(CPlayer* player, GameObj::CSkinModelRender* playerModel,
			 unsigned int shotAnim, unsigned int reloadAnim)
	:Weapon(player, playerModel, getInfo(shotAnim, reloadAnim)){
}


Rifle::~Rifle() {
}

void Rifle::createBullet(CPlayer * player, CVector3 pos, CVector3 dir) {
	NormalBullet* bul = 
		new NormalBullet(player, pos, dir*3000.0f,L"Resource/modelData/RifleBullet.cmo",
						 125, L"Resource/effect/RifleBullet.efk");
	bul->setIsThrough(true);
}

WeaponInfo Rifle::getInfo(unsigned int shotAnim, unsigned int reloadAnim) {
	WeaponInfo info;
	info.modelPath = L"Resource/modelData/Rifle.cmo";
	info.spritePath = L"Resource/spriteData/Rifle.dds";
	info.shotSEPath = L"Resource/sound/SE_rifle.wav";
	info.maxBullet = 20;
	info.shotCool = 1.0f;
	info.shotAnimNum = shotAnim;
	info.reloadAnimNum = reloadAnim;
	return info;
}
