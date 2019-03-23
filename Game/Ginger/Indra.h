#pragma once

class CPlayer;
class Game;
class GingerGene;

class Indra : public IGameObject
{
public:
	Indra(CPlayer* owner, GingerGene* gg);
	~Indra();

	bool Start()override;
	void Update()override;
	void PostRender()override;

	void Thunder(const CVector3& pos);

private:
	GameObj::CSkinModelRender m_model;
	CSprite m_flash;

	int m_cnt = 0;
	float m_flashTimer = 0.2f;

	Game* m_game = nullptr;
	CPlayer* m_owner = nullptr;
	GingerGene* m_gingergene = nullptr;
};

