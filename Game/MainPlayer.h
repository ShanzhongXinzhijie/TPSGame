#pragma once
#include "CPlayer.h"
#include "TpsCamera.h"

class MainPlayer :public CPlayer {
public:
	MainPlayer(int pad);
	~MainPlayer();
	void Update() override;

private:
	const int padNum;
	TpsCamera m_camera;
};

