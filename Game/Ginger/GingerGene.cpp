#include "stdafx.h"
#include "GingerGene.h"

GingerGene::GingerGene()
{
}
GingerGene::~GingerGene()
{
	Release();
}

void GingerGene::Register(IGameObject* go){
	m_deleteList.push_back(go);
}
void GingerGene::Release(){
	for(auto& go : m_deleteList){
		delete go;
	}
}

void GingerGene::Create(float gameTimelimit, NetPlayerReceiver* receiver){
	bool typelist[GodPowerType::enNum] = {};

	for (int i = 0; i < floor(gameTimelimit / 60.0f); i++)
	{
		int type = rand() % GodPowerType::enNum;
		if (typelist[type]) {
			for (type = 0; type < GodPowerType::enNum; type++) {
				if (!typelist[type]) { break; }
			}
			if (type == GodPowerType::enNum) { return; }
		}
		typelist[type] = true;

		Ginger* pGinger = nullptr;
		if(i == 0){
			pGinger = new Ginger(i, receiver, 30 + 10 * (rand() % 101 * 0.01f), (GodPowerType)type);
		}
		else {
			pGinger = new Ginger(i, receiver, 60 * i + 40 * (rand() % 101 * 0.01f), (GodPowerType)type);
		}
		m_gingerList.emplace_back(pGinger);
	}
}