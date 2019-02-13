#pragma once
#include <unordered_set>

class CPlayer;

class Team {
public:
	Team(CVector4 color, const wchar_t* name);
	~Team();

	CVector4 getColor() const{
		return color;
	}

	const wchar_t* getName() const {
		return name;
	}

	unsigned int getZombieCount() const{
		return zombieCount;
	}

	void addPlayer(CPlayer* player) {
		players.insert(player);
	}

	void addZombie() {
		zombieCount++;
	}

	void removeZombie() {
		zombieCount--;
	}

	void setZombie(int cnt) {
		zombieCount = cnt;
	}

	bool hasPlayer(CPlayer* player) const{
		return players.count(player) > 0;
	}

private:
	unsigned int zombieCount = 0;
	CVector4 color;
	const wchar_t* name;
	std::unordered_set<CPlayer*> players;
};

