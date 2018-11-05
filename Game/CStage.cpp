#include "stdafx.h"
#include "CStage.h"

bool CStage::Start() {
	//地形モデル
	AddGO(&m_model);
	m_model.Init(L"Resource/modelData/testback2.cmo");
	m_model.SetScale({ 10.0f, 10.0f, 10.0f });
	//判定
	CMatrix offsetmat;
	offsetmat.MakeScaling({ 10.0f, 10.0f, 10.0f });
	m_phyStaticObject.CreateMeshObject(m_model.GetSkinModel(), CVector3::Zero(), CQuaternion::Identity(), &offsetmat);

	//空
	AddGO(&m_sky);
	m_sky.Init(L"Resource/modelData/skydome.cmo");
	m_sky.SetScale({ 10.0f, 10.0f, 10.0f });
	m_sky.SetIsShadowCaster(false);

	//ライト
	m_dirlight[0].SetDirection({ -0.5f, 0.5f, 1.0f });
	m_dirlight[0].SetColor({ 0.8f, 0.8f ,0.8f });
	AddGO(&m_dirlight[0]);
	//m_dirlight[1] = AddGO(new CDirectionLight({ 0.0f,0.0f ,1.0f }, { 0.5f, -0.5f, -1.0f }));
	//m_dirlight[2] = AddGO(new CDirectionLight({ 0.0f,1.0f ,0.0f }, { 1.0f, -0.0f, 0.0f }));
	//m_dirlight[3] = AddGO(new CDirectionLight({ 1.0f,0.0f ,0.0f }, { -1.0f, -0.0f, 0.0f }));
	
	/*
	for (auto& L : m_poilight) {
		CVector3 ranvec = { CMath::Lerp(CMath::RandomZeroToOne(), -500.0f, 500.0f) ,CMath::Lerp(CMath::RandomZeroToOne(), -500.0f, 500.0f) ,CMath::Lerp(CMath::RandomZeroToOne(), -500.0f, 500.0f) };
		//L = AddGO(new CPointLight({ 1.0f*CMath::RandomZeroToOne(), 1.0f*CMath::RandomZeroToOne() ,1.0f*CMath::RandomZeroToOne() }, ranvec, 250.0f, 1.0f));
	}
	*/
	m_poilight.emplace_back(CVector3( 0.0f, 1.0f ,0.0f ), CVector3( 0.0f,10.0f ,0.0f ), 250.0f, 1.0f);
	

	GetEngine().GetGraphicsEngine().GetLightManager().SetAmbientLight({0.2f,0.3f,0.5f});

	return true;
};