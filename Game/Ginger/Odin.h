#pragma once

class CPlayer;
class Game;
class GingerGene;

class Odin : public IGameObject
{
public:
	Odin(CPlayer* owner, GingerGene* gg, const CVector3& pos);
	~Odin();

	bool Start()override;
	void Update()override;

private:
	GameObj::CSkinModelRender m_model;

	CVector3 m_pos;

	Game* m_game = nullptr;
	CPlayer* m_owner = nullptr;
	GingerGene* m_gingergene = nullptr;
};

