#include "stdafx.h"
#include "Game.h"
#include "Ground.h"
#include "CircleWalk.h"
#include "sikakukuidou.h"
#include "ziguzagu.h"
#include "ittarikitari.h"

Game::Game(/*std::unordered_map<int, CVector4>& playersIni*/){

	level.Init(L"Resource/Level/level.tkl", [&](LevelObjectData& objData)->bool {
		if (objData.EqualObjectName(L"unityChan")) {

			/*players.push_back(
				mainPlayer = new MainPlayer(0,{1,0,0,1}, objData.position)
			);
			playersMap[0] = mainPlayer;
			for (auto p : playersIni) {
				players.push_back(
#ifdef SpritScreen
					playersMap[p.first] = new MainPlayer(p.first ,p.second, objData.position)
#else
					playersMap[p.first] = new CPlayer(p.first,p.second, objData.position)
#endif
				);
				
			}*/
			playerPos = objData.position;

		} else if (objData.EqualObjectName(L"Ground")) {

			ground = new Ground(objData.position);

		}
		return true;
	});

	createPlayer(true, 1);

	citizen = std::make_unique<Citizen>(players, new ittarikitari());

#ifdef SpritScreen
	karicamera.SetPos({ 600,600,600 });
	karicamera.SetTarget({0, 0, 0});
	karicamera.SetFar(3000);
	GetCameraList().push_back(&karicamera);
	GetCameraList().push_back(&karicamera);
#endif
}

Game::~Game() {
	delete mainPlayer;
	for (const std::pair<int, CPlayer*>& ppp : playersMap) {
		delete ppp.second;
	}
	delete ground;
}

void Game::createPlayer(bool isMe, int playerNum) {
	if (isMe) {
		playersMap[playerNum] = new MainPlayer(playerNum, { 1,1,1,1 }, playerPos);
	} else {
		playersMap[playerNum] = new CPlayer(playerNum, { 1,1,1,1 }, playerPos);
	}
}

void Game::removePlayer(int playerNum) {
	delete playersMap[playerNum];
	playersMap.erase(playerNum);
}
