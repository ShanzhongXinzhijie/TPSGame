#pragma once
#include "Wosiris.h"
#include "Indra.h"
#include "IDamagable.h"

class NetPlayerReceiver;

enum GodPowerType {
	enWosiris,
	enIndra,
	enHades,
	enOdin,
	enNum,
	enNone,
};

class Ginger :
	public IGameObject, public IDamagable
{
public:
	Ginger(int index, NetPlayerReceiver* receiver, int timeSec, GodPowerType powerType);
	~Ginger();

	bool Start()override;
	void Update()override;

	void Destory();

	GodPowerType GetPowerType()const {
		return m_powerType;
	}

	bool damage(const CVector3& dir, unsigned int damage,
		const Team* team = nullptr, const CPlayer* = nullptr) override { return true; };

private:
	void Konryu();

	int m_index = -1;
	NetPlayerReceiver* m_receiver;

	int m_isKensetued = false;
	int m_kensetuLeftTime = 0, m_kensetuLeftTimeMax = 0;
	float m_uvScroll = 0.0f;
	
	int m_hp = 3000;
	GodPowerType m_powerType = enNone;
	bool m_isWillDestroy = false;

	CVector3 m_pos;
	CQuaternion m_rot;

	GameObj::CSkinModelRender m_model;
	PhysicsStaticObject m_phyStaticObject;
	SuicideObj::CCollisionObj m_collision[2];
};

