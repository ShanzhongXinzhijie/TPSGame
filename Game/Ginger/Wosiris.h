#pragma once

class CPlayer;
class Game;

class Wosiris : public IGameObject
{
public:
	Wosiris(Game* pGame, CPlayer* owner, float rot);
	~Wosiris();

	bool Start()override;
	void Update()override;
	void PostRender()override;

private:
	GameObj::CSkinModelRender m_model;
	SkinModel m_atari;
	PhysicsStaticObject m_phyStaticObject;
	SuicideObj::CCollisionObj m_collision; 

	float m_rot;

	unsigned int m_point = 0;
	CPlayer* m_owner = nullptr;

	Game* m_game = nullptr;
	std::list<std::pair<CPlayer*,bool>> m_players;

	float m_flashTimer = 0.2f;
	CFont m_font;
	CSprite m_window;
};

