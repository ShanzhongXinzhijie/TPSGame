#pragma once

#include "Ginger.h"

class GingerGene
{
public:
	GingerGene();
	~GingerGene();

	void Register(IGameObject* go);
	void Unregister(IGameObject* go);
	void Release();

	void Create(float gameTimelimit, NetPlayerReceiver* receiver);

	Ginger* GetGinger(int index) {
		return m_gingerList[index];
	}

	void SetWosiris(Wosiris* wo) { m_wosiris = wo; }
	Wosiris* GetWosiris() { return m_wosiris; }

	void SetIndra(Indra* wo) { m_indra = wo; }
	Indra* GetIndra() { return m_indra; }

private:
	std::vector<Ginger*> m_gingerList;
	std::list<IGameObject*> m_deleteList;
	Wosiris* m_wosiris = nullptr;
	Indra* m_indra = nullptr;
	bool m_isNowRelease = false;
};

