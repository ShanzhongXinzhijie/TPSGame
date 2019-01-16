#include "stdafx.h"
#include "Game.h"
#include "Ground.h"
#include "CircleWalk.h"
#include "sikakukuidou.h"
#include "ziguzagu.h"
#include "ittarikitari.h"
#include "Result.h"

Game::Game() : citizenGene(this){

	level.Init(L"Resource/Level/level.tkl", [&](LevelObjectData& objData)->bool {
		if (objData.EqualObjectName(L"player1")) {
			playerGene.addSpawner(objData.position, 780.0f);
		} else if (objData.EqualObjectName(L"player2")) {
			playerGene.addSpawner(objData.position, 780.0f);
		} else if (objData.EqualObjectName(L"zombie1")) {
			citizenGene.addSpawner(objData.position, 4600.0f);
		} else if (objData.EqualObjectName(L"zombie2")) {
			citizenGene.addSpawner(objData.position, 4600.0f);
		}
		return true;
	});
	ground = new Ground(CVector3::Zero());
#ifdef SpritScreen
	createPlayer(true, 0);
	createPlayer(true, 1);
#else
	//�v���C���[�}�l�[�W���[�̏�����(�v���C���[�̍쐬)
	m_netPlayerManager.Init(this);
#endif

	citizenGene.createCitizen(2);

#ifdef SpritScreen
	karicamera.SetPos({ 600,600,600 });
	karicamera.SetTarget({0, 0, 0});
	karicamera.SetFar(3000);
	GetCameraList().push_back(&karicamera);
	GetCameraList().push_back(&karicamera);
#endif
}

Game::~Game() {
	delete ground;
}

void Game::Update() {
	timer -= GetDeltaTimeSec();
	if (timer < 0) {
		new Result(playerGene);
		delete this;
	}
}

void Game::PostRender() {
	wchar_t countDisp[8];
	swprintf_s(countDisp, L"�c��%.1f�b", timer);
	font.Draw(countDisp, { 0.1f, 0.05f });
}

void Game::createPlayer(bool isMe, int playerNum) {
	playerGene.createPlayer(isMe, playerNum);
}

void Game::removePlayer(int playerNum) {
	playerGene.removePlayer(playerNum);
}

