#pragma once
#include "PlayerGene.h"

class Fade;
class teamResult;

class Result : public IGameObject{
public:
	Result(const PlayerGene& playerGene, Fade* fade);
	~Result();

	bool Start() override;
	void Update() override;
	void PostRender() override;

private:
	std::vector<teamResult> teamResults;

	SuicideObj::CBGM* bgm;

	GameObj::OrthoCamera camera;
	CFont m_font;
	CSprite m_sprite;
	Fade* fade;
};

