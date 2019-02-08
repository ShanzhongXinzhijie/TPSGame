#include "stdafx.h"
#include "Include\Result.h"
#include "Title.h"
#include "ConfirmPlayers.h"
#include "Team.h"
#include "Fade.h"

class teamResult {
public:
	teamResult(const wchar_t* name, unsigned int count) : name(name), count(count) {};

	bool operator <(const teamResult& tr) const{
		return count < tr.count;
	}
	bool operator >(const teamResult& tr) const{
		return count > tr.count;
	}
	const wchar_t * name;
	unsigned int count;
};

Result::Result(const PlayerGene & playerGene, Fade* fade) {
	this->fade = fade;
	fade->fadeOut();
	for (Team* t : playerGene.getTeams()) {
		teamResults.emplace_back(t->getName(), t->getZombieCount());
	}
	camera.SetPos({ 0, 0, -100 });
	camera.SetTarget({ 0, 0, 0 });
	camera.UpdateMatrix();

	bgm = NewGO<SuicideObj::CBGM>(L"Resource/sound/BGM_result.wav");
	bgm->Play(false, true);

#ifdef SpritScreen
	GetCameraList().clear();
	GetCameraList().push_back(&camera);
	GetCameraList().push_back(&camera);
#else
	SetMainCamera(&camera);
#endif
}

Result::~Result() {
	bgm->Stop();
}

bool Result::Start() {
	m_sprite.Init(L"Resource/spriteData/Title.dds");
	return true;
}

void Result::Update() {
	if (Pad(0).GetDown(enButtonA)) {
		NewGO<SuicideObj::CSE>(L"Resource/sound/SE_select.wav")->Play();
		fade->fadeIn([&]() {
			delete this;
			new ConfirmPlayers(fade);
		});
	}
}

void Result::PostRender() {
	m_sprite.Draw({ 0.5f, 0.5f }, CVector2::One(), { 0.5f, 0.5f });

	m_font.Draw(L"結 果 発 表", { 0.47f,0.1f }, CVector4::White(), { 3.0f,3.0f }, { 0.5f, 0.5f });

	//ソート
	std::sort(teamResults.begin(), teamResults.end(), std::greater<teamResult>());
	wchar_t disp[20];
	for (size_t i = 0; i < teamResults.size(); i++) {
		swprintf_s(disp, L"%d位 %ls %d 体",i+1 , teamResults[i].name, teamResults[i].count);
		m_font.Draw(disp, { 0.47f,0.3f + (i*0.1f) }, CVector4::White(), { 1.5f,1.5f }, { 0.5f, 0.5f });
	}
}
