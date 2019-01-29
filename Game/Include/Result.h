#pragma once
#include "PlayerGene.h"

class Fade;

class Result : public IGameObject{
public:
	Result(const PlayerGene& playerGene, Fade* fade);
	~Result();

	bool Start() override;
	void Update() override;
	void PostRender() override;

private:
	std::vector<const wchar_t*> names;
	std::vector<unsigned int> counts;

	GameObj::OrthoCamera camera;
	CFont m_font;
	CSprite m_sprite;
	Fade* fade;
};

