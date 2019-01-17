#pragma once
#include "MainPlayer.h"
#include "Ground.h"
#include "DemolisherWeapon/level/Level.h"
#include <unordered_set>
#include <unordered_map>
#include "Citizen.h"
#include "SqSpawner.h"
#include "Team.h"
#include "PlayerGene.h"
#include "CitizenGene.h"

#include "../Network/NetPlayerManager.h"

class Game : public IGameObject{
public:
	Game(/*std::unordered_map<int, CVector4>& playersIni*/);
	~Game();

	void Update() override;
	void PostRender() override;

	void createPlayer(bool isMe,int playerNum);
	void removePlayer(int playerNum);
	CPlayer* getPlayer(int playerNum) { 
		return playerGene.getPlayer(playerNum); 
	}
	const std::unordered_map<int, CPlayer*>& getPlayers() const {
		return playerGene.getPlayers();
	}

	const CPlayer* getMainPlayer() const{
		return playerGene.getMainPlayer();
	}

	const CPlayer* getMainPlayer2() const {
		return playerGene.getMainPlayer2();
	}

private:
	Ground* ground = nullptr;
	Level level;

	float timer = 60.0f;

#ifdef SpritScreen
	GameObj::PerspectiveCamera karicamera;
#else
	NetPlayerManager m_netPlayerManager;
#endif

	PlayerGene playerGene;
	CitizenGene citizenGene;

	CFont font;
};

