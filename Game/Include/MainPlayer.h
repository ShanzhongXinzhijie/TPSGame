#pragma once
#include "CPlayer.h"
#include "TpsCamera.h"
#include "HPbar.h"
#include "WeaponHolder.h"
#include "../Ginger/GodPowerStarter.h"

class MainPlayer :public CPlayer {
public:
	MainPlayer(int pad, Team* team, const CVector3& position);
	~MainPlayer();

	void PreUpdate() override;
	void Update() override;

	void PostRender() override;

	const TpsCamera& getCamera() const{
		return m_camera;
	}

	int GetUsePadNum()const { return playerNum; }

	void SetGodPower(GodPowerType type)override{
		godPowerStarter.SetPowertype(type);
		//�G�t�F�N�g
		SuicideObj::CEffekseer* effe = new SuicideObj::CEffekseer(L"Resource/effect/aura.efk", 1.0f, getPosition());
		effe->SetScale({ 50.0f,50.0f ,50.0f });
	}

private:
	const int playerNum = 0;
	HPbar hpbar;
	CFont font;
	TpsCamera m_camera;

	WeaponHolder wepHolder;
	bool m_isCanShortPushY = true; float m_pushYButtonTime = 0.0f;

	GodPowerStarter godPowerStarter;
};

