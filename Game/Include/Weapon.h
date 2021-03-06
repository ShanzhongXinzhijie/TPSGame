#pragma once

class CPlayer;
class Bullet;

struct WeaponInfo{
	const wchar_t* modelPath;
	const wchar_t* spritePath;
	const wchar_t* shotSEPath = L"Resource/sound/SE_shot.wav";
	unsigned int shotAnimNum;
	unsigned int reloadAnimNum;
	unsigned int maxBullet;
	float shotCool;
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

	/// <summary>
	/// アクティブ化する。
	/// </summary>
	/// <remarks>
	/// アクティブにするとモデルが描画される。
	/// </remarks>
	void Activate() {
		isActive = true;
		weaponModel.SetIsDraw(true);
	}

	void Inactivate() {
		isActive = false;
		weaponModel.SetIsDraw(false);
	}
	int getBulletCount()const {
		return bulletCount;
	}

	void setBulletCount(int bulletCount) {
		this->bulletCount = bulletCount;
	}

	bool isReloading() const{
		return reloading;
	}

	CSprite& getSpriteRef() {
		return weaponSprite;
	}

	void PreUpdate() override;

	void Update() override;

protected:
	virtual void createBullet(CPlayer* player, CVector3 pos, CVector3 dir) = 0;
	virtual void WeaponUpdate() {};
	virtual void PreShot() {};

	CPlayer* player;
	GameObj::CSkinModelRender* playerModel;
	GameObj::CSkinModelRender weaponModel;
	CSprite weaponSprite;

	const wchar_t* shotSE;

	const unsigned int shotAnimNum;
	const unsigned int reloadAnimNum;

	const float c_ShotCool = 0.1f;
	float shotCool = c_ShotCool;

	const unsigned int maxBullet;
	unsigned int bulletCount = maxBullet;

	bool reloading;

	bool isActive = false;
};

