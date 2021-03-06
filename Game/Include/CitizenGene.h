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

	Citizen* GetCitizen(unsigned int index) {
		if (index >= citizenArray.size()) { return nullptr; }
		return citizenArray[index];
	}
	const Citizen* GetCitizen(unsigned int index)const {
		if (index >= citizenArray.size()) { return nullptr; }
		return citizenArray[index];
	}
	size_t GetCitizenNum()const {
		return citizenArray.size();
	}

private:
	/// <summary>
	/// (updatePeriod)回に一回Updateを行う
	/// </summary>
	static constexpr unsigned char updatePeriod = 10;
	unsigned char nowFlame = updatePeriod;

	float allSize = 0;

	Game* game;
	std::vector<Citizen*> citizenArray;
	std::vector<SqSpawner*> spawnerArray;
};

