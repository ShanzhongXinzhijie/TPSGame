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
	/// (updatePeriod)��Ɉ��Update���s��
	/// </summary>
	static constexpr unsigned char updatePeriod = 5;
	unsigned char nowFlame = updatePeriod;

	Game* game;
	std::vector<Citizen*> citizenArray;
	std::vector<SqSpawner*> spawnerArray;
};
