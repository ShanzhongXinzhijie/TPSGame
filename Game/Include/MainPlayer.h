#pragma once
#include "CPlayer.h"
#include "TpsCamera.h"
#include "HPbar.h"

class MainPlayer :public CPlayer {
public:
	MainPlayer(int pad, CVector4 color, const CVector3& position);
	~MainPlayer();
	void Update() override;

	void PostRender() override;

private:
	const int playerNum;
	HPbar hpbar;
	TpsCamera m_camera;
};

