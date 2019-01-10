#pragma once
#include "MainPlayer.h"
#include "Ground.h"
#include "DemolisherWeapon/level/Level.h"
#include <unordered_set>
#include <unordered_map>
#include "Citizen.h"
#include "SqSpawner.h"
#include "Team.h"

#include "../Network/NetPlayerManager.h"

class Game : public IGameObject{
public:
	Game(/*std::unordered_map<int, CVector4>& playersIni*/);
	~Game();

	void Update() override;
	void PostRender() override;

	void createPlayer(bool isMe,int playerNum);
	void removePlayer(int playerNum);
	CPlayer* getPlayer(int playerNum) { return playersMap[playerNum]; };

private:
	Ground* ground = nullptr;
	Level level;
	std::unordered_map<int, CPlayer*> playersMap;

	float timer = 30.0f;

#ifdef SpritScreen
	GameObj::PerspectiveCamera karicamera;
#else
	NetPlayerManager m_netPlayerManager;
#endif

	SqSpawner playerSpawn1;
	SqSpawner playerSpawn2;
	SqSpawner citizenSpawn1;
	SqSpawner citizenSpawn2;

	Team redTeam;
	Team blueTeam;
	bool nextIsRed = false;

	std::vector<Citizen*> citizenArray;

	CFont font;
};

