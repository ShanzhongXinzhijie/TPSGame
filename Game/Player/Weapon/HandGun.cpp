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

void HandGun::createBullet(CPlayer * player, CVector3 pos, CVector3 dir) {
	const float angle = 20.0f;
	const int count = 3;
	CQuaternion rot;
	rot.SetRotationDeg(CVector3::AxisY(), -(angle*(count-1)/2));
	rot.Multiply(dir);
	rot.SetRotationDeg(CVector3::AxisY(), angle);
	for (int i = 0; i < count; i++) {
		new NormalBullet(player, pos, dir*3000.0f, L"Resource/modelData/Bat.cmo", 50);
		rot.Multiply(dir);
	}
}

WeaponInfo HandGun::getInfo(unsigned int shotAnim, unsigned int reloadAnim) {
	WeaponInfo info;
	info.modelPath = L"Resource/modelData/HundGun.cmo";
	info.spritePath = L"Resource/spriteData/HundGun.dds";
	info.maxBullet = 100;
	info.shotCool = 0.2f;
	info.shotAnimNum = shotAnim;
	info.reloadAnimNum = reloadAnim;
	return info;
}
