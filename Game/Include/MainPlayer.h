#pragma once
#include "CPlayer.h"
#include "TpsCamera.h"

class MainPlayer :public CPlayer {
public:
	MainPlayer(int pad, CVector4 color, const CVector3& position);
	~MainPlayer();
	void Update() override;

private:
	const int playerNum;
	bool pushB = false;
	TpsCamera m_camera;
};

