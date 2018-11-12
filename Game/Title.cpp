#include "stdafx.h"
#include "Title.h"
#include "ConfirmPlayers.h"

Title::Title() {
}


Title::~Title() {
}

bool Title::Start() {
	m_render.Init(L"Resource/modelData/Title.cmo");
	m_render.SetPos({ 0,0,0 });
	m_render.SetScale({ 2.2f, 2, 2 });
	m_dirlight.SetDirection({ 0.0f, 0.0f, -1.0f });
	m_dirlight.SetColor({ 1, 1 ,1 });

	camera.SetPos({ 0, 0, -100 });
	camera.SetTarget({0, 0, 0});
	camera.UpdateMatrix();
	SetMainCamera(&camera);
	return true;
}

void Title::Update() {
	if (Pad(0).GetButton(enButtonA)) {
		new ConfirmPlayers;
		delete this;
	}
}