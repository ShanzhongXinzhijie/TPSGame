#include "stdafx.h"
#include "Ground.h"

Ground::Ground(CVector3 pos)  : m_pos(pos){

}

bool Ground::Start() {
	m_skyModel.Init(L"Resource/modelData/sky.cmo");
	m_skyModel.SetIsMostDepth(true);
	m_skyModel.SetIsShadowCaster(false);

	m_model.Init(L"Resource/modelData/Ground.cmo");
	m_model.SetPos(m_pos);
	m_model.SetIsShadowCaster(true);
	m_phyStaticObject.CreateMesh(m_model);

	m_dirlight.SetDirection({ 0.5f, -1.0f, 0.5f });
	m_dirlight.SetColor({ 0.8f, 0.8f ,0.8f });

	shadowMap.Init(2048, 2048, m_dirlight.GetDirection());
	shadowMap.SetArea({ 10000.0f,10000.0f,50000.0f });
	shadowMap.SetTarget({0, 2500.0f, 0});

	return true;
}

void Ground::Update() {
	m_skyModel.SetPos(GetMainCamera()->GetPos());
}