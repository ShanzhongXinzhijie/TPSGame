#include "stdafx.h"
#include "Include\Result.h"
#include "Title.h"
#include "Team.h"
#include "Fade.h"

Result::Result(const PlayerGene & playerGene, Fade* fade) {
	this->fade = fade;
	fade->fadeOut();
	for (Team* t : playerGene.getTeams()) {
		names.push_back(t->getName());
		counts.push_back(t->getZombieCount());
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

Result::~Result() {
}

bool Result::Start() {
	m_sprite.Init(L"Resource/spriteData/Title.dds");
	return true;
}

void Result::Update() {
	if (Pad(0).GetDown(enButtonA)) {
		fade->fadeIn([&]() {
			delete this;
			new Title(fade);
		});
	}
}

void Result::PostRender() {
	m_sprite.Draw({ 0.5f, 0.5f }, CVector2::One(), { 0.5f, 0.5f });

	m_font.Draw(L"Œ‹ ‰Ê ”­ •\", { 0.47f,0.1f }, CVector4::White(), { 3.0f,3.0f }, { 0.5f, 0.5f });

	unsigned int maxCount = 0;
	size_t winerTeam = 0;
	wchar_t disp[20];
	for (size_t i = 0; i < names.size(); i++) {
		if (maxCount < counts[i]) {
			maxCount = counts[i];
			winerTeam = i;
		}
		swprintf_s(disp, L"%ls %d ‘Ì", names[i], counts[i]);
		m_font.Draw(disp, { 0.47f,0.3f + (i*0.1f) }, CVector4::White(), { 1.5f,1.5f }, { 0.5f, 0.5f });
	}

	swprintf_s(disp, L"%ls ‚ÌŸ—˜", names[winerTeam]);

	m_font.Draw(disp, { 0.47f, 0.8f }, CVector4::White(), { 1.5f,1.5f }, { 0.5f, 0.5f });
}
