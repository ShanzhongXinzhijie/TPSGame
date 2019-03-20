#include "stdafx.h"
#include "GingerGene.h"
#include "Ginger.h"

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

void GingerGene::Create(float gameTimelimit){
	bool typelist[GodPowerType::enNum] = {};

	for (int i = 0; i < floor(gameTimelimit / 60.0f); i++)
	{
		int type = rand() % GodPowerType::enNum;
		if (typelist[type]) {
			for (int i2 = 0; i2 < GodPowerType::enNum; i2++) {
				type = i2;
				if (!typelist[i2]) { break; }
			}
			if (type == GodPowerType::enNum) { return; }
		}
		typelist[type] = true;

		Ginger* pGinger = nullptr;
		if(i == 0){
			pGinger = new Ginger(30 + 10 * CMath::RandomZeroToOne(), (GodPowerType)type);
		}
		else {
			pGinger = new Ginger(60 * i + 40 * CMath::RandomZeroToOne(), (GodPowerType)type);
		}
		Register(pGinger);
	}
}