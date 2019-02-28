#pragma once

class CPlayer;

class Weapon : public IGameObject{
public:
	Weapon(CPlayer* player, GameObj::CSkinModelRender* playerModel);
	~Weapon();

	virtual void shot() = 0;

	virtual void drawBulletCount(CFont& font) = 0;

	virtual void reload() = 0;

	bool isReloading() const{
		return reloading;
	};

	float getZoomScale() const {
		return zoomScale;
	}

protected:
	CPlayer* player;
	GameObj::CSkinModelRender* playerModel;

	bool reloading;

	float zoomScale = 1.0f;
};

