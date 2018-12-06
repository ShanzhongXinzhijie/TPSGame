#pragma once
#include "MainPlayer.h"
#include "Ground.h"
#include "DemolisherWeapon/level/Level.h"
#include <unordered_set>
#include <unordered_map>
#include "Citizen.h"

class Game : public IGameObject{
public:
	Game(/*std::unordered_map<int, CVector4>& playersIni*/);
	~Game();

	void createPlayer(bool isMe,int playerNum);
	void removePlayer(int playerNum);

private:
	MainPlayer* mainPlayer = nullptr;

	Ground* ground = nullptr;
	Level level;
	std::unordered_map<int, CPlayer*> playersMap;
	std::vector<CPlayer*> players;

#ifdef SpritScreen
	GameObj::PerspectiveCamera karicamera;
#else
	NetWorkManager* m_p_networkManager = nullptr;
#endif

	CVector3 playerPos;
	std::unique_ptr<Citizen> citizen;
};

