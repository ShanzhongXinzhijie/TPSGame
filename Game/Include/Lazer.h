#pragma once
#include "Weapon.h"

class Game;
class CPlayer; class Citizen;

class Lazer :
	public Weapon
{
public:
	Lazer(CPlayer* player, GameObj::CSkinModelRender* playerModel,
		unsigned int shotAnimNum, unsigned int reloadAnimNum);
	~Lazer();

	void PostRender()override;

private:
	void createBullet(CPlayer* player, CVector3 pos, CVector3 dir);
	static WeaponInfo getInfo(unsigned int shotAnim, unsigned int reloadAnim);
	void WeaponUpdate()override;
	void PreShot()override;

	void LockOn();
	CVector3 GetLockPos()const;

	//int m_isPly = true, m_num = -1; 
	//CPlayer* m_pPly = nullptr; const Citizen* m_pCiti = nullptr;
	//CVector3 m_lockPos;

	float m_charge = 0.0f;

	CSprite m_sprite[2]; 
	SuicideObj::CEffekseer* m_eff = nullptr;
	bool m_isOnEFF = false; float m_effCnt = 0.0f;

	SuicideObj::CSE* m_se = nullptr;

	Game* m_game = nullptr;
};

