#pragma once
#include"Ginger.h"

class GingerGene;
class Game;
class MainPlayer;

class GodPowerStarter : public IGameObject
{
public:
	GodPowerStarter();
	~GodPowerStarter();

	void Init(MainPlayer* player);

	void SetPowertype(GodPowerType type);

	bool Start()override;
	void Update()override;
	void PostRender()override;

private:
	GodPowerType m_type = enNone;

	GameObj::CSkinModelRender m_ring; 
	int m_ringTime = 0; int m_mesageTime = 0;
	CFont m_font, m_fontDefo;
	wchar_t m_text[256] = L"";
	CSprite m_bButton;

	GingerGene* m_pGinGene = nullptr;
	Game* m_pGame = nullptr;
	MainPlayer* m_pPlayer = nullptr;
};

