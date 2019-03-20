#pragma once

#include "Ginger.h"

class GingerGene
{
public:
	GingerGene();
	~GingerGene();

	void Register(IGameObject* go);
	void Release();

	void Create(float gameTimelimit, NetPlayerReceiver* receiver);

	Ginger* GetGinger(int index) {
		return m_gingerList[index];
	}

private:
	std::vector<Ginger*> m_gingerList;
	std::list<IGameObject*> m_deleteList;
};

