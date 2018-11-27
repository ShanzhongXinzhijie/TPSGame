#include "stdafx.h"
#include "Game.h"
#include "Ground.h"
#include "CircleWalk.h"
#include "sikakukuidou.h"
#include "ziguzagu.h"
#include "ittarikitari.h"

Game::Game(std::unordered_set<int> playersIni){
	level.Init(L"Resource/Level/level.tkl", [&](LevelObjectData& objData)->bool {
		if (objData.EqualObjectName(L"unityChan")) {
			players.push_back(
				mainPlayer = new MainPlayer(0, objData.position)
			);
			playersMap[0] = mainPlayer;
			for (int num : playersIni) {
				players.push_back(
					playersMap[num] = new MainPlayer(num, objData.position)
				);
				
			}
		} else if (objData.EqualObjectName(L"Ground")) {
			ground = new Ground(objData.position);
		}
		return true;
	});

	citizen = std::make_unique<Citizen>(players, new ittarikitari());

	karicamera.SetPos({ 600,600,600 });
	karicamera.SetTarget({0, 0, 0});
	karicamera.SetFar(3000);
	GetCameraList().push_back(&karicamera);
	GetCameraList().push_back(&karicamera);
}


Game::~Game() {
	delete mainPlayer;
	for (const std::pair<int, CPlayer*>& ppp : playersMap) {
		delete ppp.second;
	}
	delete ground;
}
