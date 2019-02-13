#pragma once

class Citizen;
class SqSpawner;
class Game;

class CitizenGene : public IGameObject{
public:
	CitizenGene(Game* game);
	~CitizenGene();

	void PreUpdate() override;

	void addSpawner(const CVector3& center, float side);

	void createCitizen(unsigned int amount);

private:
	/// <summary>
	/// (updatePeriod)‰ñ‚Éˆê‰ñUpdate‚ğs‚¤
	/// </summary>
	static constexpr unsigned char updatePeriod = 10;
	unsigned char nowFlame = updatePeriod;

	float allSize = 0;

	Game* game;
	std::vector<Citizen*> citizenArray;
	std::vector<SqSpawner*> spawnerArray;
};

