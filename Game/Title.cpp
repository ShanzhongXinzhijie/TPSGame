#include "stdafx.h"
#include "Title.h"
#include "ConfirmPlayers.h"
#include "Fade.h"

Title::Title(Fade* fade, SuicideObj::CBGM* bgm) : bgm(bgm){
	this->fade = fade;
	fade->fadeOut();

	if (bgm == nullptr) {
		this->bgm = NewGO<SuicideObj::CBGM>(L"Resource/sound/BGM_title.wav");
		this->bgm->Play(false, true);
	}

	camera.SetPos({ 0, 0, -100 });
	camera.SetTarget({ 0, 0, 0 });
	camera.UpdateMatrix();
#ifdef SpritScreen
	GetCameraList().clear();
	GetCameraList().push_back(&camera);
	GetCameraList().push_back(&camera);
#else
	SetMainCamera(&camera);
#endif
}


Title::~Title() {
}

bool Title::Start() {
	m_sprite.Init(L"Resource/spriteData/Title.dds");
	return true;
}

void Title::Update() {
	if (Pad(0).GetDown(enButtonA) && fade->isIdel()) {
		NewGO<SuicideObj::CSE>(L"Resource/sound/SE_select.wav")->Play();
		fade->fadeIn([&]() {
			new ConfirmPlayers(fade, bgm);
			delete this;
		});
	}
}

void Title::PostRender() {
	m_sprite.Draw({ 0.5f, 0.5f }, CVector2::One(), {0.5f, 0.5f});
	m_font.Draw(L"Vanpire", { 0.47f,0.3f }, CVector4::White(), { 3.0f,3.0f }, { 0.5f, 0.5f });
	m_font.Draw(L"push A to Start", { 0.47f, 0.8f }, CVector4::White(), { 1.5f,1.5f }, { 0.5f, 0.5f });
}