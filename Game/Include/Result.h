#pragma once
#include "PlayerGene.h"

class Result : public IGameObject{
public:
	Result(const PlayerGene& playerGene);
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
};

