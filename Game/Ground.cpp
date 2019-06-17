#include "stdafx.h"
#include "Ground.h"

Ground::Ground(CVector3 pos)  : m_pos(pos){

}

bool Ground::Start() {
	m_skyModel.Init(L"Resource/modelData/sky.cmo");
	m_skyModel.SetIsMostDepth(true);
	m_skyModel.SetIsShadowCaster(false);
	m_skyModel.SetDrawPriority(DRAW_PRIORITY_MAX);
	
	m_model.Init(L"Resource/modelData/Ground.cmo");
	m_model.SetPos(m_pos);
	m_model.SetIsShadowCaster(true);
	m_phyStaticObject.CreateMesh(m_model);

	m_dirlight.SetDirection({ 0.5f, -1.0f, 0.5f });
	m_dirlight.SetColor({ 0.4f, 0.4f ,0.8f });

	//‰Šú‰»
	float split[3] = { 0.0f,0.048f,0.8f };
	CVector2 kaizoudo[2]; kaizoudo[0] = { 2048,2048 }; kaizoudo[1] = { 1024,1024 };
	CascadeShadowmap.Init(2,//•ªŠ„”
		split, kaizoudo
	);
	CascadeShadowmap.SetDirection(m_dirlight.GetDirection());
	CascadeShadowmap.SetNear(50.0f);
	CascadeShadowmap.SetFar(50000.0f);

	//ŠÂ‹«Œõ
	SetAmbientCubeMap(L"Resource/cubemap/IBL.dds", 0.5f);

	return true;
}

void Ground::Update() {
	m_skyModel.SetPos(GetMainCamera()->GetPos());
}