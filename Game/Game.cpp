#include "stdafx.h"
#include "Game.h"
#include "Ground.h"
#include "CircleWalk.h"
#include "sikakukuidou.h"
#include "ziguzagu.h"
#include "ittarikitari.h"

Game::Game(){

	level.Init(L"Resource/Level/level.tkl", [&](LevelObjectData& objData)->bool {
		if (objData.EqualObjectName(L"unityChan")) {
			playerPos = objData.position;

		} else if (objData.EqualObjectName(L"Ground")) {

			ground = new Ground(objData.position);

		}
		return true;
	});

#ifdef SpritScreen
	createPlayer(true, 1);
#else
	//プレイヤーマネージャーの初期化(プレイヤーの作成)
	m_netPlayerManager.Init(this);
#endif

	citizen = std::make_unique<Citizen>(playersMap, new ittarikitari());

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
	//重複を許さない
	if (playersMap.count(playerNum) > 0) {
		return;
	}

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
