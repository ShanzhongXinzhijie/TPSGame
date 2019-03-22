#pragma once
#include "IDamagable.h"

class CPlayer;
class Game;
class GingerGene;

class Wosiris : public IGameObject, public IDamagable
{
public:
	Wosiris(CPlayer* owner, float rot, GingerGene* gg);
	~Wosiris();

	bool Start()override;
	void Update()override;
	void PostRender()override;

	void ChangeControl(CPlayer* P);

	bool damage(const CVector3& dir, unsigned int damage,
		const Team* team = nullptr, const CPlayer* = nullptr) override { return true; };

private:
	GameObj::CSkinModelRender m_model;
	SkinModel m_atari;
	PhysicsStaticObject m_phyStaticObject;
	SuicideObj::CCollisionObj m_collision; 

	float m_rot;

	unsigned int m_point = 0;
	CPlayer* m_owner = nullptr;

	Game* m_game = nullptr;
	GingerGene* m_gingergene = nullptr;
	std::list<std::pair<CPlayer*,bool>> m_players;

	float m_flashTimer = 0.2f;
	CFont m_font;
	CSprite m_window;

//í êMóp
	int m_lastControlPly = -1, m_lastControlTime = INT_MIN;
public:
	void SetLastControl(int ply, int time) {
		m_lastControlPly = ply;
		m_lastControlTime = time;
	}
	int GetLastControlPly()const { return m_lastControlPly; }
	int GetLastControlTime()const { return m_lastControlTime; }
};

