#pragma once

class CPlayer;

class Weapon : public IGameObject{
public:
	Weapon(CPlayer* player,
		   GameObj::CSkinModelRender* playerModel,
		   const wchar_t* weaponModelPath,
		   float zoomScale = 1.0f);
	~Weapon();

	virtual void shot() = 0;

	virtual void drawBulletCount(CFont& font) = 0;

	virtual void reload() = 0;

	virtual void setBulletCount(int) = 0;
	virtual int  getBulletCount()const = 0;

	bool isReloading() const{
		return reloading;
	};

	float getZoomScale() const {
		return zoomScale;
	}

	void Update() override final;

protected:
	CPlayer* player;
	GameObj::CSkinModelRender* playerModel;
	GameObj::CSkinModelRender weaponModel;

	bool reloading;

	const float zoomScale;
};

