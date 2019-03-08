#include "stdafx.h"
#include "WeaponHolder.h"
#include "Weapon.h"

WeaponHolder::WeaponHolder(Weapon* weapons[], size_t weaponsSize) :
	weapons(weapons), weaponsSize(weaponsSize){
	YButton.Init(L"Resource/spriteData/YButton.dds");
	LButton.Init(L"Resource/spriteData/LButton.dds");
	RButton.Init(L"Resource/spriteData/RButton.dds");
}

WeaponHolder::~WeaponHolder() {
}

void WeaponHolder::draw() {
	const float screenX = GetEngine().GetGraphicsEngine().GetFrameBuffer_W();
	const float screenY = GetEngine().GetGraphicsEngine().GetFrameBuffer_H();
	const CVector2 pos = { screenX * 0.5f, screenY * 0.1f };

	float l_slide = 0;
	if (slide != 0) {
		l_slide = GetDeltaTimeSec() * (slide > 0 ? 1 : -1);
		if (fabsf(slide) < fabsf(l_slide)) {
			slide = 0;
		} else {
			slide -= l_slide;
		}
	}

	size_t wNum = (weaponsSize + activeWeapon - 2) % weaponsSize;

	for (int i = -2; i < 3; i++) {
		int absi = i;
		int sign = 1;
		if (absi < 0) {
			absi *= -1;
			sign = -1;
		}

		float side = i * 150;
		float scale = 1 - i * 0.2;
		weapons[wNum]->getSpriteRef()
			.DrawScreenPos({ pos.x + side, pos.y + 95.0f }, {scale,scale}, { 0.5f,0.5f });
		

		wNum++;
		if (wNum == weaponsSize) {
			wNum = 0;
		}
	}

	YButton.DrawScreenPos({ pos.x, pos.y + 210.0f }, CVector3::One(), { 0.5f,0.5f });
	LButton.DrawScreenPos({ pos.x - 230.0f, pos.y + 216.0f }, CVector3::One(), { 0.5f,0.5f });
	RButton.DrawScreenPos({ pos.x + 230.0f, pos.y + 216.0f }, CVector3::One(), { 0.5f,0.5f });
}

void WeaponHolder::changeWeapon(bool left, bool right) {
	if (left == right) { return; }
	short nextWeapon = activeWeapon;
	if (left) { nextWeapon -= 1; slide = 1;}
	if (right) { nextWeapon += 1; slide = -1;}
	if (nextWeapon < 0) { nextWeapon = weaponsSize - 1; }
	if (nextWeapon >= weaponsSize) { nextWeapon = 0; }
	activeWeapon = nextWeapon;
}
