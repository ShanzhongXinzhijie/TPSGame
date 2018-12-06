#include "stdafx.h"
#include "Game.h"
#include "Ground.h"
#include "CircleWalk.h"
#include "sikakukuidou.h"
#include "ziguzagu.h"
#include "ittarikitari.h"

Game::Game(std::unordered_map<int, CVector4>& playersIni){

	level.Init(L"Resource/Level/level.tkl", [&](LevelObjectData& objData)->bool {
		if (objData.EqualObjectName(L"unityChan")) {

			players.push_back(
				mainPlayer = new MainPlayer(0,{1,0,0,1}, objData.position)
			);
			playersMap[0] = mainPlayer;
			for (auto p : playersIni) {
				players.push_back(
#ifdef SpritScreen
					playersMap[p.first] = new MainPlayer(p.first ,p.second, objData.position)
#else
					playersMap[p.first] = new CPlayer(p.second, objData.position)
#endif
				);
				
			}

		} else if (objData.EqualObjectName(L"Ground")) {

			ground = new Ground(objData.position);

		}
		return true;
	});

	citizen = std::make_unique<Citizen>(players, new ittarikitari());

#ifdef SpritScreen
	karicamera.SetPos({ 600,600,600 });
	karicamera.SetTarget({0, 0, 0});
	karicamera.SetFar(3000);
	GetCameraList().push_back(&karicamera);
	GetCameraList().push_back(&karicamera);
#else
	m_p_networkManager = FindGO<NetWorkManager>();
	m_p_networkManager->setActionFunc([&](const ActionSender& ac, int pn) {
		playersMap[pn]->sendAction(ac);
	});
#endif
}

Game::~Game() {
	delete mainPlayer;
	for (const std::pair<int, CPlayer*>& ppp : playersMap) {
		delete ppp.second;
	}
	delete ground;
}
