#pragma once
#include"../Network/NetworkConst.h"

class CPlayer;
class Game;
class GingerGene;
class OdinPower;

class OdinPowerHUD : public IGameObject{
public:
	OdinPowerHUD(OdinPower*oya) : m_oya(oya){}

	bool Start()override;
	void PostRender()override;
private:
	CSprite m_gauge[2];
	OdinPower* m_oya = nullptr;
};

class OdinPower : public IGameObject, public IRander
{
public:
	OdinPower() : m_hud(this){
		m_priority = GetGraphicsEngine().GetFreeRenderPriority();
		GetGraphicsEngine().GetRenderManager().AddRender(m_priority, this);
	}
	~OdinPower(){
		if (m_depthStencilState) { m_depthStencilState->Release(); }
		//if (m_RSCw) { m_RSCw->Release(); }if (m_RSCCw) { m_RSCCw->Release(); }
		if (m_blendState) { m_blendState->Release(); }
		GetGraphicsEngine().GetRenderManager().DeleteRender(m_priority);
	}

	void Init(Game* pGame){
		m_game = pGame;
	}

	bool Start()override;
	void Update()override;
	void Render()override;

	void SetTime(CPlayer* player);
	float GetTimePersent(int index)const{
		return (float)m_time[index] / TIME_MAX;
	}

private:
	static constexpr int TIME_MAX = 900;
	int m_time[NET_MAX_PLAYER] = {};
	Game* m_game = nullptr;
	int m_priority = -1;

	ID3D11DepthStencilState* m_depthStencilState = nullptr;	
	//ID3D11RasterizerState* m_RSCw = nullptr, *m_RSCCw = nullptr;
	ID3D11BlendState* m_blendState = nullptr;
	Shader m_ps;

	OdinPowerHUD m_hud;
};

class Odin : public IGameObject
{
public:
	Odin(CPlayer* owner, GingerGene* gg, const CVector3& pos);
	~Odin();

	bool Start()override;
	void Update()override;

private:
	GameObj::CSkinModelRender m_model;
	PhysicsStaticObject m_phyStaticObject;

	CVector3 m_pos;
	OdinPower m_odinPower;

	Game* m_game = nullptr;
	CPlayer* m_owner = nullptr;
	GingerGene* m_gingergene = nullptr;
};

