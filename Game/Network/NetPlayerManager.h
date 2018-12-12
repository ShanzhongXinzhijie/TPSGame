#pragma once

#include"NetPlayerReceiver.h"
#include"NetPlayerCaster.h"

class Game;

class NetPlayerManager : public IGameObject
{
public:
	NetPlayerManager();
	~NetPlayerManager();

	void Init(Game* pGame);

	void PostRender()override;

private:
	void JoinEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player);
	void LeaveEventAction(int playerNr, bool isInactive);

	void CreatePlayer(int playerNr);
	
	Game* m_game = nullptr;
	NetPlayerReceiver* m_netReceiver = nullptr;
	std::unordered_map<int, NetPlayerCaster*> m_playerCastersMap;

	CFont m_font;
};

