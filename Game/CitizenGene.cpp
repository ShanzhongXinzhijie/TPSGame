#include "stdafx.h"
#include "Include\CitizenGene.h"
#include "Include\SqSpawner.h"
#include "Include\Citizen.h"
#include "ittarikitari.h"
#include "Game.h"

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
		Citizen* citizen = new Citizen(game->getPlayers() , new ittarikitari());
		citizenArray.push_back(citizen);
		citizen->setPos(spawnerArray[i%spawnCount]->getPos());
	}
}
