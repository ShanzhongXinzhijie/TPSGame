#pragma once

class Weapon;

class WeaponHolder {
public:
	WeaponHolder(Weapon* weapons[], size_t weaponsSize);
	~WeaponHolder();

	void draw();

	void changeWeapon(bool left, bool right);

private:
	const size_t weaponsSize;
	Weapon** weapons;//Weapon‚Ì”z—ñ
	size_t activeWeapon = 0;
	float slide = 0;

	constexpr static float m_scale = 0.6f;

	const float screenX = GetEngine().GetGraphicsEngine().GetFrameBuffer_W();
	const float screenY = GetEngine().GetGraphicsEngine().GetFrameBuffer_H();
	const CVector2 m_pos = { screenX * 0.15f, screenY * 0.82f };

	CSprite LButton;
	CSprite RButton;
};

