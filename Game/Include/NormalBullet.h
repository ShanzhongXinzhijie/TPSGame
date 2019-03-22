#pragma once
#include "DemolisherWeapon/physics/CollisionAttr.h"
#include "Bullet.h"

class CPlayer;

class EfkDeleter :public IGameObject {
public:
	EfkDeleter(SuicideObj::CEffekseer* effect) : efk(effect) {
		efk->SetPaused(true);
	}
	void Update() override {
		count -= GetDeltaTimeSec();
		if (count > 0) {
			uint8_t c = 255;
			c = uint8_t(c * count);
			efk->SetColor({ 255,255,255,c});
		} else {
			delete efk;
			delete this;
		}
	}
private:
	SuicideObj::CEffekseer* efk = nullptr;
	float count = 1.0f;
};

class NormalBullet : public Bullet{
public:
	NormalBullet(CPlayer* player, CVector3 position, CVector3 direction,
				 const wchar_t* modelPath, unsigned int damage, const wchar_t* efkPath = nullptr);
	~NormalBullet();

	void BulletUpdate() override;

private:
	SuicideObj::CEffekseer* efk = nullptr;
};

