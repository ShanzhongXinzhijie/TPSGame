#include "stdafx.h"
#include "Game.h"
#include "Ground.h"
#include "CircleWalk.h"
#include "sikakukuidou.h"
#include "ziguzagu.h"
#include "ittarikitari.h"
#include "Result.h"

Game::Game() : redTeam({ 1,0.5f,0.5f,1 }), blueTeam({ 0.5f,0.5f,1,1 }),
playerSpawn1(780.0f), playerSpawn2(780.0f) {
	SqSpawner citizenSpawn1(4600.0f);
	SqSpawner citizenSpawn2(4600.0f);

	level.Init(L"Resource/Level/level.tkl", [&](LevelObjectData& objData)->bool {
		if (objData.EqualObjectName(L"player1")) {
			playerSpawn1.setPos(objData.position);
		} else if (objData.EqualObjectName(L"player2")) {
			playerSpawn2.setPos(objData.position);
		} else if (objData.EqualObjectName(L"zombie1")) {
			citizenSpawn1.setPos(objData.position);
		} else if (objData.EqualObjectName(L"zombie2")) {
			citizenSpawn2.setPos(objData.position);
		}
		return true;
	});
	ground = new Ground(CVector3::Zero());
#ifdef SpritScreen
	createPlayer(true, 0);
	createPlayer(true, 1);
#else
	//プレイヤーマネージャーの初期化(プレイヤーの作成)
	m_netPlayerManager.Init(this);
#endif

	for (int i = 0; i < 24; i++) {

		Citizen* citizen = new Citizen(playersMap, new ittarikitari());
		citizenArray.push_back(citizen);
		if (i > 12) {
			citizen->setPos(citizenSpawn1.getPos());
		} else {
			citizen->setPos(citizenSpawn2.getPos());
		}
	}

#ifdef SpritScreen
	karicamera.SetPos({ 600,600,600 });
	karicamera.SetTarget({0, 0, 0});
	karicamera.SetFar(3000);
	GetCameraList().push_back(&karicamera);
	GetCameraList().push_back(&karicamera);
#endif
}

Game::~Game() {
#ifdef SpritScreen
	for (const std::pair<int, CPlayer*>& ppp : playersMap) {
		delete ppp.second;
	}
#endif
	for (Citizen* cp : citizenArray) {
		delete cp;
	}
	delete ground;
}

void Game::Update() {
	timer -= GetDeltaTimeSec();
	if (timer < 0) {
		new Result(redTeam.getZombieCount(), blueTeam.getZombieCount());
		delete this;

	}
}

void Game::PostRender() {
	wchar_t countDisp[8];
	swprintf_s(countDisp, L"残り%.1f秒", timer);
	font.Draw(countDisp, { 0.1f, 0.05f });
}

void Game::createPlayer(bool isMe, int playerNum) {
	//重複を許さない
	if (playersMap.count(playerNum) > 0) {
		return;
	}

	CVector3 v = playerSpawn1.getPos();

	Team* t;
	SqSpawner* sSpawn;
	if (playerNum%2 == 0) {
		t = &redTeam;
		sSpawn = &playerSpawn1;
	} else {
		t = &blueTeam;
		sSpawn = &playerSpawn2;
	}

	if (isMe) {
		playersMap[playerNum] = new MainPlayer(playerNum, t, sSpawn->getPos());
	} else {
		playersMap[playerNum] = new CPlayer(playerNum, t, sSpawn->getPos());
	}
}

void Game::removePlayer(int playerNum) {
	delete playersMap[playerNum];
	playersMap.erase(playerNum);
}

