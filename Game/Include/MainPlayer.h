#pragma once
#include "CPlayer.h"
#include "TpsCamera.h"

#include "Network/Network.h"

class MainPlayer :public CPlayer {
public:
	MainPlayer(int pad, CVector4 color, const CVector3& position);
	~MainPlayer();
	void Update() override;

private:
	const int padNum;
	bool pushB = false;
	TpsCamera m_camera;

	//�ʐM�p
	NetActionSenderCaster* m_actionSenderCaster = nullptr;
	NetWorkManager* m_p_networkManager = nullptr;
};

