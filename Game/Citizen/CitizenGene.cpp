#include "stdafx.h"
#include "CitizenGene.h"
#include "SqSpawner.h"
#include "Citizen.h"
#include "Game.h"

#include "CircleWalk.h"
#include "ittarikitari.h"
#include "sikakukuidou.h"
#include "ziguzagu.h"
#include "Citizennigeru.h"


CitizenGene::CitizenGene(Game * game) : game(game){
}

CitizenGene::~CitizenGene() {
	for (Citizen* c : citizenArray) {
		delete c;
	}
}

void CitizenGene::PreUpdate() {
	if (nowFlame == updatePeriod) {
		const CPlayer* mp = game->getMainPlayer();
		const CPlayer* mp2 = game->getMainPlayer2();

		unsigned char delay = updatePeriod;

		constexpr float length = 500.0f;
		for (Citizen* cp : citizenArray) {
			if ((cp->getPos() - mp->getPosition()).LengthSq() < length * length) {
				continue;
			}
			if (mp2 != nullptr && (cp->getPos() - mp2->getPosition()).LengthSq() < length * length) {
				continue;
			}

			cp->setUpdateDelay(delay, updatePeriod);
			if (delay > 1) {
				delay--;
			} else {
				delay = updatePeriod;
			}
		}
	}

	if (1 < nowFlame) {
		nowFlame--;
	} else {
		nowFlame = updatePeriod;
	}
}

void CitizenGene::addSpawner(const CVector3 & center, float side) {
	spawnerArray.push_back(new SqSpawner(side, center));
	allSize += side * side;
}

void CitizenGene::createCitizen(unsigned int amount) {
	size_t spawnCount = spawnerArray.size();
	float nowSize = allSize;
	unsigned int id = 0;
	for (SqSpawner* sp : spawnerArray) {

		unsigned int createNum;
		{
			float spSize = sp->getSide();
			spSize *= spSize;
			createNum = (spSize / nowSize) * amount;
			nowSize -= spSize;
			amount -= createNum;
		}

		for (unsigned int i = 0; i < createNum; i++) {
			ICitizenBrain* brain;
			switch (rand() % 5) {
			case 0:
				brain = new CircleWalk();
				break;
			case 1:
				brain = new ittarikitari();
				break;
			case 2:
				brain = new sikakukuidou();
				break;
			case 3:
				brain = new Citizennigeru(game->getPlayers());
				break;
			default:
				brain = new ziguzagu();
			}
			Citizen* citizen = new Citizen(game->getPlayers(), brain, id); 
			id++;
			citizenArray.push_back(citizen);
			citizen->setPos(sp->getPos());
		}
	}
}
