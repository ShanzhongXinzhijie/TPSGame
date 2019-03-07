#pragma once

class CPlayer;
class Bullet;

struct WeaponInfo{
	const wchar_t* modelPath;
	unsigned int shotAnimNum;
	unsigned int reloadAnimNum;
	unsigned int maxBullet;
	float shotCool;
	float zoomScale = 1.0f;
};

class Weapon : public IGameObject{
public:
	Weapon(CPlayer* player,
		   GameObj::CSkinModelRender* playerModel,
		   const WeaponInfo& info);
	~Weapon();

	void shot();

	void drawBulletCount(CFont& font);

	void reload();

	void Activate() {
		isActive = true;
		weaponModel.SetIsDraw(true);
	}

	void Inactivate() {
		isActive = false;
		weaponModel.SetIsDraw(false);
	}

	virtual void setBulletCount(int) = 0;
	virtual int  getBulletCount()const = 0;

	bool isReloading() const{
		return reloading;
	};

	float getZoomScale() const {
		return zoomScale;
	}

	void PreUpdate() override;

	void Update() override;

protected:
	virtual Bullet* createBullet(CPlayer* player, CVector3 pos, CVector3 dir) = 0;

	CPlayer* player;
	GameObj::CSkinModelRender* playerModel;
	GameObj::CSkinModelRender weaponModel;

	const float zoomScale;

	const unsigned int shotAnimNum;
	const unsigned int reloadAnimNum;

	const float c_ShotCool = 0.1f;
	float shotCool = c_ShotCool;

	const unsigned int maxBullet;
	unsigned int bulletCount = maxBullet;

	bool reloading;

	bool isActive = false;
};

