#include "stdafx.h"
#include "Game.h"


Game::Game(std::unordered_set<int> playersIni) : mainPlayer(0){
	for (int num : playersIni) {
		playersMap[num] = new MainPlayer(num);
	}
}


Game::~Game() {
	for (const std::pair<int, CPlayer*>& ppp : playersMap) {
		delete ppp.second;
	}
}
