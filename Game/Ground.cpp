#include "stdafx.h"
#include "Ground.h"

Ground::Ground(CVector3 pos)  : m_pos(pos){

}

bool Ground::Start() {
	m_model.Init(L"Resource/modelData/Ground.cmo");
	m_model.SetPos(m_pos);
	m_phyStaticObject.CreateMesh(m_model);

	m_dirlight[0].SetDirection({ 0.5f, 1.0f, 0.5f });
	m_dirlight[0].SetColor({ 0.8f, 0.8f ,0.8f });

	m_dirlight[1].SetDirection({ -0.5f, -1.0f, -0.5f });
	m_dirlight[1].SetColor({ 0.3f, 0.3f ,0.3f });
	return true;
}
