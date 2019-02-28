#pragma once
#include "Weapon.h"

class HandGun : public Weapon{
public:
	HandGun(CPlayer* player, GameObj::CSkinModelRender* playerModel,
			unsigned int shotAnimationNum, unsigned int reloadAnimationNum);
	~HandGun();

	void PreUpdate() override;
	void shot() override;
	void reload() override;
	void drawBulletCount(CFont& font) override;

private:
	const unsigned int shotAnimationNum;
	const unsigned int reloadAnimationNum;

	static constexpr float constShotCool = 0.1f;
	float shotCool = constShotCool;

	static constexpr unsigned int maxBulletCount = 50;
	unsigned int bulletCount = maxBulletCount;
};

