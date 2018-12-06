#include "stdafx.h"
#include "ConfirmPlayers.h"
#include "CPlayer.h"
#include "Title.h"
#include "Game.h"

#include "Network/Network.h"

ConfirmPlayers::ConfirmPlayers() : list(L"Resource/spriteData/waku.dds") {
	camera.SetPos({ 0, 50, 200 });
	camera.SetTarget({ 0, 50, 0 });
	camera.UpdateMatrix();

#ifdef SpritScreen
	GetCameraList().clear();
	GetCameraList().push_back(&camera);
	GetCameraList().push_back(&camera);
#else
	SetMainCamera(&camera);
#endif
}


ConfirmPlayers::~ConfirmPlayers() {
}

bool ConfirmPlayers::Start() {

	m_dirlight.SetColor({ 1,1,1 });
	m_dirlight.SetDirection({ 0,0,1 });

	m_sprite.Init(L"Resource/spriteData/Title.dds");

	return true;
}

void ConfirmPlayers::Update() {
#ifdef SpritScreen
	for (int num = 1; num < 4; num++) {

		if (Pad(num).GetDown(enButtonStart)) {
			if (players.count(num) == 0) {
				players[num] = { 1,0,0,1 };
			} else {
				players.erase(num);
			}
		}
	}
#endif

	if (Pad(0).GetDown(enButtonStart)) {
#ifdef SpritScreen
		GetCameraList().clear();
#endif
		new Game();
		delete this;
	} else if (Pad(0).GetDown(enButtonBack)) {
		m_caster.Send(false,false);
		new Title;
		delete this;
	}
}

void ConfirmPlayers::PostRender() {
	m_sprite.Draw({ 0.5f, 0.5f }, CVector2::One(), { 0.5f, 0.5f });
	list.Draw();
}
