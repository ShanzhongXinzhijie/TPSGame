#include "stdafx.h"
#include "BatBullet.h"


BatBullet::BatBullet(CVector3 position, CVector3 direction) : m_pos(position), m_dir(direction){
}


BatBullet::~BatBullet() {
}

bool BatBullet::Start() {
	m_model.Init(L"Resource/modelData/Bat.cmo");
	m_model.SetPos(m_pos);
	CQuaternion rot;
	rot.SetRotation(CVector3::AxisY(), atan2f(m_dir.x, m_dir.z));

	float xz = m_dir.x*m_dir.x + m_dir.z*m_dir.z;
	xz = sqrt(xz);

	rot.Multiply(CQuaternion::GetRotation(CVector3::AxisX(), atan2f(-m_dir.y, xz)));
	m_model.SetRot(rot);
	m_model.SetScale({ 0.5f, 0.5f, 0.5f });
	return true;
}

void BatBullet::Update() {
	m_pos += m_dir;
	m_model.SetPos(m_pos);
	lifeTime -= GetDeltaTimeSec();
	if (lifeTime < 0) {
		delete this;
	}
}
