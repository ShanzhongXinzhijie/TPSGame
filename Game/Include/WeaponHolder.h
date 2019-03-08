#pragma once

class Weapon;

class WeaponHolder{
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

	CSprite YButton;
	CSprite LButton;
	CSprite RButton;
};

