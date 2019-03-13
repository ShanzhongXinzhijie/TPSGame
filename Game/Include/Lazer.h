#pragma once
#include "Weapon.h"

class Game;

class Lazer :
	public Weapon
{
public:
	Lazer(CPlayer* player, GameObj::CSkinModelRender* playerModel,
		unsigned int shotAnimNum, unsigned int reloadAnimNum);
	~Lazer();

	void PostRender()override;

private:
	Bullet* createBullet(CPlayer* player, CVector3 pos, CVector3 dir);
	static WeaponInfo getInfo(unsigned int shotAnim, unsigned int reloadAnim);
	void WeaponUpdate()override;

	int m_isPly = true, m_num = -1;
	CVector3 m_lockPos;

	CSprite m_sprite;

	Game* m_game = nullptr;
};

