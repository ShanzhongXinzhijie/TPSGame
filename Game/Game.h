#pragma once
#include "MainPlayer.h"
#include "Ground.h"
#include <unordered_set>
#include <unordered_map>

class Game : public IGameObject{
public:
	Game(std::unordered_set<int> playersIni);
	~Game();
private:
	Ground ground;
	MainPlayer mainPlayer;
	std::unordered_map<int, CPlayer*> playersMap;
};

