#pragma once
#include <unordered_set>

class CPlayer;

class Team {
public:
	Team(CVector4 color);
	~Team();

	void addPlayer(CPlayer* player) {
		players.insert(player);
	}

	CVector4 getColor() const{
		return color;
	}

	void addZombie() {
		zombieCount++;
	}

	void removeZombie() {
		zombieCount--;
	}

	unsigned int getZombieCount() {
		return zombieCount;
	}

	bool hasPlayer(CPlayer* player) {
		return players.count(player) > 0;
	}

private:
	unsigned int zombieCount = 0;
	CVector4 color;
	std::unordered_set<CPlayer*> players;
};

