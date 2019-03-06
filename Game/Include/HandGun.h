#pragma once
#include "Weapon.h"

class HandGun : public Weapon{
public:
	HandGun(CPlayer* player, GameObj::CSkinModelRender* playerModel,
			unsigned int shotAnimNum, unsigned int reloadAnimNum);
	~HandGun();

	void PreUpdate() override;
	void shot() override;
	void reload() override;
	void drawBulletCount(CFont& font) override;
	
	void setBulletCount(int cnt)override {
		bulletCount = cnt;
	}
	int getBulletCount()const override {
		return bulletCount;
	}

private:
	const unsigned int shotAnimNum;
	const unsigned int reloadAnimNum;

	static constexpr float constShotCool = 0.1f;
	float shotCool = constShotCool;

	static constexpr unsigned int maxBulletCount = 50;
	unsigned int bulletCount = maxBulletCount;
};

