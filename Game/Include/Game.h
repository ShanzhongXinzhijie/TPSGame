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
#include "DemolisherWeapon/Graphic/Light/Lights.h"

#include "../Network/NetPlayerManager.h"
#include "../Network/NetGameEventCaster.h"
#include "../Network/NetWork.h"

class Fade;

class Game : public IGameObject{
public:
	static Game* createGame(Fade* fade, float timeLimit, int citizenCnt, int seed, int startTime_ms) {
		if (static_game == nullptr) {
			return static_game = new Game(fade, timeLimit, citizenCnt, seed, startTime_ms);
		}
		return nullptr;
	}

	static const Game* getGame() {
		return static_game;
	}

private:
	Game(Fade* fade, float timeLimit, int citizenCnt, int seed, int startTime_ms);
public:
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

	const MainPlayer* getMainPlayer() const{
		return playerGene.getMainPlayer();
	}

	const MainPlayer* getMainPlayer2() const {
		return playerGene.getMainPlayer2();
	}

	const CitizenGene& GetCitizenGene()const {
		return citizenGene;
	}

private:
	static Game* static_game;

	GameObj::CDirectionLight light;
	Ground* ground = nullptr;
	Level level;

	float m_waitGameStartTimer_sec = 0.0f;
	float timer = 160.0f;

	PlayerGene playerGene;
	CitizenGene citizenGene;

#ifdef SpritScreen
	GameObj::PerspectiveCamera karicamera;
#else
	NetPlayerManager m_netPlayerManager;
	NetGameEventCaster m_netEventCaster;
	NetWorkManager* m_netWork = nullptr;
#endif

	SuicideObj::CBGM* bgm;
	CFont font;
	Fade* fade;
};

