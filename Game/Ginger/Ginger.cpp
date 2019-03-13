#include "stdafx.h"
#include "Ginger.h"
#include "CollisionMaskConst.h"

Ginger::Ginger(int time)
{
	m_kensetuLeftTime = time;
}
Ginger::~Ginger()
{
}

bool Ginger::Start() {
	m_model.Init(L"Resource\\modelData\\Ginger.cmo", enFbxUpAxisY);
	m_pos = { 2863.38f - 2863.38f*2.0f*(rand() % 101 * 0.01f), 0.0f, 2863.38f - 2863.38f*2.0f*(rand() % 101 * 0.01f) };
	//レイで判定
	m_model.SetPos(m_pos);
	m_rot.SetRotationDeg(CVector3::AxisY(), 360.0f*(rand() % 101 * 0.01f));
	m_model.SetRot(m_rot);

	//地形判定
	m_phyStaticObject.CreateMesh(m_model);

	//暗い判定	
	for (int i = 0; i < 2; i++) {
		if (i == 0) {
			CVector3 offset = CVector3( 0.0f, 376.115f, -815.025f )*0.21f; m_rot.Multiply(offset);
			m_collision[i].CreateBox(m_pos + offset, m_rot, CVector3( 376.115f*1.2f,376.115f,376.115f*1.2f )*0.21f*2.0f);
		}
		else {
			CVector3 offset = CVector3( 0.0f, 671.355f, -0.0f )*0.21f; m_rot.Multiply(offset);
			m_collision[i].CreateBox(m_pos + offset, m_rot, CVector3( 479.25f*1.2f,671.355f,73.04f )*0.21f*2.0f);
		}
		m_collision[i].SetName(L"Ginger");
		m_collision[i].SetClass(this);
		//マスクとグループの設定
		m_collision[i].All_Off_Group();
		m_collision[i].On_OneGroup(CollisionMaskConst::encolKurai);
		m_collision[i].Off_OneMask(CollisionMaskConst::encolKurai);
		//これは喰らい判定
		m_collision[i].SetIsHurtCollision(true);
	}

	m_kensetuLeftTime = (int)(m_kensetuLeftTime*(rand() % 101 * 0.01f));

	return true;
}
void Ginger::Update() {

}