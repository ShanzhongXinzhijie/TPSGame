#include "stdafx.h"
#include "PlayerGene.h"
#include "SqSpawner.h"
#include "Citizen.h"
#include "MainPlayer.h"
#include "Team.h"


PlayerGene::PlayerGene() {
	teamArray.push_back(new Team({ 1,0.0f,0.0f,1 }, L"赤チーム", this));
	teamArray.push_back(new Team({ 0.0f,0.0f,1,1 }, L"青チーム", this));
}


PlayerGene::~PlayerGene() {
#ifdef SpritScreen
	for (const std::pair<int, CPlayer*>& ppp : playersMap) {
		delete ppp.second;
	}
#endif
	for (Team* t : teamArray) {
		delete t;
	}
}

void PlayerGene::addSpawner(const CVector3 & center, float side) {
	spawnerArray.push_back(new SqSpawner(side, center));
}

void PlayerGene::createPlayer(bool isMe, int playerNum) {
	//重複を許さない
	if (playersMap.count(playerNum) > 0) {
		return;
	}

	size_t teamCount = teamArray.size();
	Team* t = teamArray[playerNum%teamCount];
	SqSpawner* sSpawn = spawnerArray[playerNum%teamCount%spawnerArray.size()];
	if (isMe) {
		CPlayer* mp = new MainPlayer(playerNum, t, sSpawn->getPos());
		playersMap[playerNum] = mp;
		if (mainPlayer == nullptr) {
			mainPlayer = mp;
		} else {
			mainPlayer2 = mp;
		}
	} else {
		playersMap[playerNum] = new CPlayer(playerNum, t, sSpawn->getPos());
	}

}

void PlayerGene::removePlayer(int playerNum) {
	delete playersMap[playerNum];
	playersMap.erase(playerNum);
}

CVector3 PlayerGene::getHome(const Team * team) const {
	size_t teamNum = 0;
	for (; teamNum < teamArray.size(); teamNum++) {
		if (teamArray[teamNum] != team) {
			teamNum++;
		} else {
			break;
		}
	}
	return spawnerArray[teamNum%spawnerArray.size()]->getPos();
}
