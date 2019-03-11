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
	if (isPushingY) {
		toBigDelta += GetDeltaTimeSec() * 15;
		if (toBigDelta > 1.0f) {
			toBigDelta = 1.0f;
		}
	} else {
		toBigDelta += GetDeltaTimeSec() * -15;
		if (toBigDelta < 0.0f) {
			toBigDelta = 0.0f;
		}
	}
	m_scale = c_scale + toBigDelta * 0.3f;
	m_pos.Lerp(toBigDelta, fromPos, toPos);
	float alpha = 0.7 + toBigDelta * 0.3f;

	if (slide != 0) {
		float l_slide = GetDeltaTimeSec() * (slide > 0 ? 8 : -8);
		if (fabsf(slide) < fabsf(l_slide)) {
			slide = 0;
		} else {
			slide -= l_slide;
		}
	}

	size_t wNum;
	if (slide >= 0) {
		wNum = (weaponsSize + activeWeapon - 3) % weaponsSize;
	} else {
		wNum = (weaponsSize + activeWeapon - 2) % weaponsSize;
	}

	const float iconSize = 188 * m_scale;
	float scale = 1.0f / 3 * slide;
	if (slide < 0) {
		scale = 1.0f / 3 * (1+slide);
	}
	float side = -284*m_scale;
	side += iconSize * scale / 2;
	for (int i = 0; i < 6; i++) {
		if (scale > 0.01f) {
			weapons[wNum]->getSpriteRef()
				.DrawScreenPos({ m_pos.x + side, m_pos.y + 95.0f*m_scale },
							   { scale*m_scale, scale*m_scale }, { 0.5f,0.5f }, 0, {1,1,1,alpha});
		}
		side += iconSize * scale / 2;
		if (i < 2) {
			scale += 1.0f / 3;
		} else if (i == 2) {
			scale = 1 + 2.0f/3 - scale;
		}else {
			scale -= 1.0f / 3;
		}
		side += iconSize * scale / 2;

		wNum++;
		if (wNum == weaponsSize) {
			wNum = 0;
		}
	}

	YButton.DrawScreenPos({ m_pos.x, m_pos.y + 210.0f*m_scale }, {m_scale, m_scale}, { 0.5f,0.5f }, 0, { 1,1,1,alpha });
	LButton.DrawScreenPos({ m_pos.x - 230.0f*m_scale, m_pos.y + 216.0f*m_scale }, { m_scale, m_scale }, { 0.5f,0.5f }, 0, { 1,1,1,alpha });
	RButton.DrawScreenPos({ m_pos.x + 230.0f*m_scale, m_pos.y + 216.0f*m_scale }, { m_scale, m_scale }, { 0.5f,0.5f }, 0, { 1,1,1,alpha });
	isPushingY = false;
}

void WeaponHolder::changeWeapon(bool left, bool right) {
	if (left == right) { return; }
	short nextWeapon = activeWeapon;
	if (left) { nextWeapon -= 1; slide = -1;}
	if (right) { nextWeapon += 1; slide = 1;}
	if (nextWeapon < 0) { nextWeapon = weaponsSize - 1; }
	if (nextWeapon >= weaponsSize) { nextWeapon = 0; }
	activeWeapon = nextWeapon;
}
