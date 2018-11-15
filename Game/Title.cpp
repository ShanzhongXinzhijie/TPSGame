#include "stdafx.h"
#include "Title.h"
#include "ConfirmPlayers.h"

Title::Title() {
}


Title::~Title() {
}

bool Title::Start() {
	camera.SetPos({ 0, 0, -100 });
	camera.SetTarget({0, 0, 0});
	camera.UpdateMatrix();
	SetMainCamera(&camera);
	GetCameraList().clear();
	GetCameraList().push_back(&camera);
	GetCameraList().push_back(&camera);
	return true;
}

void Title::Update() {
	if (Pad(0).GetButton(enButtonA)) {
		new ConfirmPlayers;
		delete this;
	}
}

void Title::PostRender() {
	m_font.Draw(L"Vanpire", { 0.47f,0.3f }, { 0,0,0,1}, { 3.0f,3.0f }, { 0.5f, 0.5f });
	m_font.Draw(L"push A to Start", { 0.47f, 0.8f }, { 0,0,0,1 }, { 1.5f,1.5f }, { 0.5f, 0.5f });
}