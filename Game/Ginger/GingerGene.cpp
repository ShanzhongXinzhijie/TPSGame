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
	for (int i = 0; i < floor(gameTimelimit / 60.0f); i++)
	{
		Ginger* pGinger = nullptr;
		if(i == 0){
			pGinger = new Ginger(30.0f + 10.0f * CMath::RandomZeroToOne());
		}
		else {
			pGinger = new Ginger(60.0f * i + 40.0f * CMath::RandomZeroToOne());
		}
		Register(pGinger);
	}
}