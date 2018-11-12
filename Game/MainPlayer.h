#pragma once
#include "CPlayer.h"
#include "CPlayerCamera.h"

class MainPlayer :public CPlayer {
public:
	MainPlayer(int pad);
	~MainPlayer();
	void Update() override;

private:
	const int padNum;
	CPlayerCamera m_camera;
};

