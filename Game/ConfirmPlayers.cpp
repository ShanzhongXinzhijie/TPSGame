#include "stdafx.h"
#include "ConfirmPlayers.h"
#include "CPlayer.h"
#include "Title.h"
#include "Game.h"

ConfirmPlayers::ConfirmPlayers(): list(L"Resource/spriteData/waku.dds") {
}


ConfirmPlayers::~ConfirmPlayers() {
}

bool ConfirmPlayers::Start() {
	camera.SetPos({ 0, 50, 200 });
	camera.SetTarget({ 0, 50, 0 });
	GetCameraList().clear();
	GetCameraList().push_back(&camera);
	GetCameraList().push_back(&camera);
	camera.UpdateMatrix();

	m_dirlight.SetColor({ 1,1,1 });
	m_dirlight.SetDirection({ 0,0,1 });

	m_sprite.Init(L"Resource/spriteData/Title.dds");

	list.values.push_back(L"player1");

	return true;
}

void ConfirmPlayers::Update() {
	for (int num = 1; num < 4; num++) {

		if (Pad(num).GetButton(enButtonStart)) {
			if (startButton[num - 1] == false) {
				if (players.count(num) == 0) {
					players[num] = {1,0,0,1};
				} else {
					players.erase(num);
				}
			}
			startButton[num - 1] = true;
		} else {
			startButton[num - 1] = false;
		}

	}

	if (Pad(0).GetButton(enButtonStart)) {
		GetCameraList().clear();
		new Game(players);
		delete this;
	} else if(Pad(0).GetButton(enButtonBack)) {
		new Title;
		delete this;
	}
}

void ConfirmPlayers::PostRender() {
	m_sprite.Draw({ 0.5f, 0.5f }, CVector2::One(), { 0.5f, 0.5f });
	list.Draw();
}
