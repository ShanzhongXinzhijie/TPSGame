#include "stdafx.h"
#include "BatBullet.h"
#include "CPlayer.h";

BatBullet::BatBullet(int pNum ,CVector3 position, CVector3 direction)
	: m_pos(position), m_dir(direction), shotPlayerNum(pNum){
}


BatBullet::~BatBullet() {

}

bool BatBullet::Start() {
	m_model.Init(L"Resource/modelData/Bat.cmo");
	m_model.SetPos(m_pos);

	//ÉÇÉfÉãâÒì]
	CQuaternion rot;
	rot.SetRotation(CVector3::AxisY(), atan2f(m_dir.x, m_dir.z));

	float xz = m_dir.x*m_dir.x + m_dir.z*m_dir.z;
	xz = sqrt(xz);

	rot.Multiply(CQuaternion::GetRotation(CVector3::AxisX(), atan2f(-m_dir.y, xz)));
	m_model.SetRot(rot);
	m_model.SetScale({ 0.5f, 0.5f, 0.5f });

	//ÉRÉäÉWÉáÉìèâä˙âª
	m_collision = std::make_unique<SuicideObj::CCollisionObj>();
	m_collision->CreateSphere(m_pos, CQuaternion::Identity(), 15.0f);
	m_collision->SetName(L"BatBullet");
	m_collision->SetClass(this);

	m_collision->SetCallback([&](SuicideObj::CCollisionObj::SCallbackParam& callback){
		if (callback.EqualName(L"CPlayer")) {
			if (callback.GetClass<CPlayer>()->BatHit(shotPlayerNum, getHitVec())) {
				delete this;
			}
		}
	});

	return true;
}

void BatBullet::Update() {
	m_pos += m_dir;
	m_model.SetPos(m_pos);
	m_collision->SetPosition(m_pos);
	lifeTime -= GetDeltaTimeSec();
	if (lifeTime < 0) {
		delete this;
	}
}

CVector3 BatBullet::getHitVec() const{
	CVector3 v = m_dir;
	v.Normalize();
	v *= 100;
	v.y = 100;
	return v;
}
