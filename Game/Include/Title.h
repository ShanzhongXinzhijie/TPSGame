#pragma once
#include "CConfig.h"
using namespace GameObj;

class Fade;

class Title : public IGameObject{
public:
	Title(Fade* fade, SuicideObj::CBGM* bgm = nullptr);
	~Title();
	
	bool Start() override;
	void Update() override;
	void PostRender() override;

	void ConfigSave();
private:
	OrthoCamera camera;
	CFont m_font;
	CSprite m_sprite;

	SuicideObj::CBGM* bgm;

	Fade* fade;

	CConfig::ConfigData m_configData;
};

