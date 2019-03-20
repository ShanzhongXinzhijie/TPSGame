#pragma once

#include "GuiList.h"

#include"NetPlayerReceiver.h"
#include"NetPlayerCaster.h"

class Game;

class NetPlayerManager : public IGameObject
{
public:
	NetPlayerManager();
	~NetPlayerManager();

	void Init(Game* pGame, CitizenGene* citizenGene);

	void Update()override;
	void PostRender()override;

private:
	void JoinEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player);
	void LeaveEventAction(int playerNr, bool isInactive);

	void CreatePlayer(int playerNr);
	
	Game* m_game = nullptr;
	CitizenGene* m_citizenGene = nullptr;
	NetPlayerReceiver* m_netReceiver = nullptr;
	std::unordered_map<int, NetPlayerCaster*> m_playerCastersMap;

	bool m_isView = false;
	GuiList list;
	CFont m_font;
};

