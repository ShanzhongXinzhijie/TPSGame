#pragma once

class CPlayer;

class Team {
public:
	Team(CVector4 color);
	~Team();

	void addPlayer(CPlayer* player) {
		players.push_back(player);
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

private:
	unsigned int zombieCount = 0;
	CVector4 color;
	std::vector<CPlayer*> players;
};

