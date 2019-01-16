#pragma once
#include <unordered_map>

class CPlayer;
class SqSpawner;
class Team;

class PlayerGene {
public:
	PlayerGene();
	~PlayerGene();

	void addSpawner(const CVector3& center, float side);

	void createPlayer(bool isMe, int playerNum);

	void removePlayer(int playerNum);

	CPlayer* getPlayer(int playerNum) {
		return playersMap[playerNum];
	}

	const std::unordered_map<int, CPlayer*>& getPlayers() const{
		return playersMap;
	}

	std::vector<Team*> getTeams() const{
		return teamArray;
	}

private:
	std::unordered_map<int, CPlayer*> playersMap;
	std::vector<SqSpawner*> spawnerArray;
	std::vector<Team*> teamArray;
};

