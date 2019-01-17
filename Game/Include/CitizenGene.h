#pragma once

class Citizen;
class SqSpawner;
class Game;

class CitizenGene : public IGameObject{
public:
	CitizenGene(Game* game);
	~CitizenGene();

	void addSpawner(const CVector3& center, float side);

	void createCitizen(unsigned int amount);

private:
	Game* game;
	std::vector<Citizen*> citizenArray;
	std::vector<SqSpawner*> spawnerArray;
};

