#pragma once
#include "CPlayer.h"
#include "TpsCamera.h"
#include "HPbar.h"
#include "Flybar.h"

class MainPlayer :public CPlayer {
public:
	MainPlayer(int pad, Team* team, const CVector3& position);
	~MainPlayer();
	void Update() override;

	void PostRender() override;

private:
	const int playerNum = 0;
	HPbar hpbar;
	CFont font;
	TpsCamera m_camera;
	Flybar flybar;

	bool triggerL = false;
	bool triggerR = false;
};

