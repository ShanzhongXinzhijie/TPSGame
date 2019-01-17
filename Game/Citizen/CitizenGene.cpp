#include "stdafx.h"
#include "CitizenGene.h"
#include "SqSpawner.h"
#include "Citizen.h"
#include "Game.h"

#include "CircleWalk.h"
#include "ittarikitari.h"
#include "sikakukuidou.h"
#include "ziguzagu.h"


CitizenGene::CitizenGene(Game * game) : game(game){
}

CitizenGene::~CitizenGene() {
	for (Citizen* c : citizenArray) {
		delete c;
	}
}

void CitizenGene::addSpawner(const CVector3 & center, float side) {
	spawnerArray.push_back(new SqSpawner(side, center));
}

void CitizenGene::createCitizen(unsigned int amount) {
	size_t spawnCount = spawnerArray.size();
	for (unsigned int i = 0; i < amount; i++) {
		ICitizenBrain* brain;
		switch(rand() % 4) {
		case 0:
			brain = new CircleWalk();
			break;
		case 1:
			brain = new ittarikitari();
			break;
		case 2:
			brain = new sikakukuidou();
			break;
		default:
			brain = new ziguzagu();
		}
		Citizen* citizen = new Citizen(game->getPlayers() , brain);
		citizenArray.push_back(citizen);
		citizen->setPos(spawnerArray[i%spawnCount]->getPos());
	}
}
