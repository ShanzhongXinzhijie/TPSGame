#pragma once

class Weapon;

class WeaponHolder {
public:
	WeaponHolder(Weapon* weapons[], size_t weaponsSize);
	~WeaponHolder();

	void draw();

	void changeWeapon(bool left, bool right);

	void pushY() {
		isPushingY = true;
	}

private:
	const size_t weaponsSize;
	Weapon** weapons;//Weapon‚Ì”z—ñ
	size_t activeWeapon = 0;
	float slide = 0;

	constexpr static float c_scale = 0.6f;
	float m_scale = c_scale;

	const float screenX = GetEngine().GetGraphicsEngine().GetFrameBuffer_W();
	const float screenY = GetEngine().GetGraphicsEngine().GetFrameBuffer_H();
	const CVector2 toPos = {screenX * 0.25f, screenY * 0.65f};
	const CVector2 fromPos = { screenX * 0.15f, screenY * 0.78f };
	CVector2 m_pos = fromPos;

	bool isPushingY = false;
	float toBigDelta = 0.0f;

	CSprite YButton;
	CSprite LButton;
	CSprite RButton;
};

