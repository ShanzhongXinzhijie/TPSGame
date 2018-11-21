#include "stdafx.h"
#include "ConfirmPlayers.h"
#include "CPlayer.h"
#include "Title.h"
#include "Game.h"

ConfirmPlayers::ConfirmPlayers() {
	players.clear();
}


ConfirmPlayers::~ConfirmPlayers() {
	for (CSkinModelRender* model : pModels) {
		if (model != nullptr) {
			delete model;
		}
	}
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

	myModel.Init(L"Resource/modelData/unityChan.cmo");
	myModel.SetPos({200, 0, 0});

	return true;
}

void ConfirmPlayers::Update() {
	for (int num = 1; num < 4; num++) {
		if (Pad(num).GetButton(enButtonStart)) {
			if (startButton[num - 1] == false) {
				if (players.count(num) == 0) {
					players.insert(num);
					pModels[modelCount] = new CSkinModelRender;
					pModels[modelCount]->Init(L"Resource/modelData/unityChan.cmo");
					pModels[modelCount]->SetPos({ (float)(modelCount * 100), 0, 0 });
					modelCount++;
				} else {
					players.erase(num);
					modelCount--;
					delete pModels[modelCount];
					pModels[modelCount] = nullptr;
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
	} else if(Pad(0).GetButton(enButtonSelect)) {
		new Title;
		delete this;
	}
}
