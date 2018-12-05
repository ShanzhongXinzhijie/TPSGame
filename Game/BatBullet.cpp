#include "stdafx.h"
#include "BatBullet.h"
#include "CPlayer.h"
#include "Citizen.h"

using SuicideObj::CCollisionObj;

BatBullet::BatBullet(CPlayer* player,CVector3 position, CVector3 direction)
	: m_pos(position), m_dir(direction), shotPlayer(player){
}


BatBullet::~BatBullet() {

}

bool BatBullet::Start() {
	m_model.Init(L"Resource/modelData/Bat.cmo");
	m_model.SetPos(m_pos);

	//モデル回転
	CQuaternion rot;
	rot.SetRotation(CVector3::AxisY(), atan2f(m_dir.x, m_dir.z));

	float xz = m_dir.x*m_dir.x + m_dir.z*m_dir.z;
	xz = sqrt(xz);

	rot.Multiply(CQuaternion::GetRotation(CVector3::AxisX(), atan2f(-m_dir.y, xz)));
	m_model.SetRot(rot);
	m_model.SetScale({ 0.5f, 0.5f, 0.5f });

	//コリジョン初期化
	m_collision.CreateSphere(m_pos, CQuaternion::Identity(), 20.0f);
	m_collision.SetName(L"BatBullet");
	m_collision.SetClass(this);

	//弾がヒットした時の処理
	m_collision.SetCallback([&](CCollisionObj::SCallbackParam& callback){
		if (callback.EqualName(L"CPlayer")) {
			if (callback.GetClass<CPlayer>()->BatHit(shotPlayer, getHitVec())) {
				DeleteGO(this, false);
			}

		} else if (callback.EqualName(L"Citizen")) {
			if (callback.GetClass<Citizen>()->BatHit(shotPlayer, getHitVec())) {
				DeleteGO(this, false);
			}
		}
	});

	return true;
}

void BatBullet::Update() {
	CVector3 beforePos = m_pos;
	m_pos += m_dir;
	m_model.SetPos(m_pos);
	m_collision.SetPosition(m_pos);
	lifeTime -= GetDeltaTimeSec();
	if (lifeTime < 0) {
		delete this;
	}
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	start.setOrigin(btVector3(beforePos.x, beforePos.y, beforePos.z));
	end.setOrigin(btVector3(m_pos.x, m_pos.y, m_pos.z));

	Callback callback;
	GetEngine().GetPhysicsWorld().ConvexSweepTest
	((const btConvexShape*)m_collision.GetCollisionObject().getCollisionShape(), start, end, callback);
	if (callback) {
		DeleteGO(this, false);
	}
}

CVector3 BatBullet::getHitVec() const{
	CVector3 v = m_dir;
	v.Normalize();
	v *= 100;
	v.y = 100;
	return v;
}
