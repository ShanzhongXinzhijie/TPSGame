#pragma once
#include "MainPlayer.h"
#include "Ground.h"
#include "DemolisherWeapon/level/Level.h"
#include <unordered_set>
#include <unordered_map>
#include "Citizen.h"

class Game : public IGameObject{
public:
	Game(std::unordered_set<int> playersIni);
	~Game();
private:
	MainPlayer* mainPlayer = nullptr;

	Ground* ground = nullptr;
	Level level;
	std::unordered_map<int, CPlayer*> playersMap;

	GameObj::PerspectiveCamera karicamera;
	Citizen citizen;
};

