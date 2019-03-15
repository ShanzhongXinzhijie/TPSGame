#include "stdafx.h"
#include "Bullet.h"
#include "CPlayer.h"
#include "Citizen.h"

using SuicideObj::CCollisionObj;

Bullet::Bullet(CPlayer* player, CVector3 position, CVector3 direction,
			   const wchar_t* modelPath, unsigned int damage)
	: m_pos(position), m_dir(direction), shotPlayer(player), damage(damage){
	if (modelPath) { m_model.Init(modelPath); }
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
	m_collision.SetName(L"Bullet");
	m_collision.SetClass(this);

	//弾がヒットした時の処理
	m_collision.SetCallback([&](CCollisionObj::SCallbackParam& callback) {
		if (callback.EqualName(L"CPlayer")) {
			if (callback.GetClass<CPlayer>()->BatHit(this)) {
				DeleteGO(this, false);
			}

		} else if (callback.EqualName(L"Citizen")) {
			if (callback.GetClass<Citizen>()->BatHit(this)) {
				DeleteGO(this, false);
			}
		}
	});
}


Bullet::~Bullet() {

}

void Bullet::Update() {
	lifeTime -= GetDeltaTimeSec();
	if (lifeTime < 0) {
		delete this; return;
	}

	CVector3 beforePos = m_pos;
	m_pos += m_dir * GetDeltaTimeSec();
	m_model.SetPos(m_pos);
	m_collision.SetPosition(m_pos);

	btVector3 startVec(beforePos.x, beforePos.y, beforePos.z);
	btVector3 endVec(m_pos.x, m_pos.y, m_pos.z);
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	start.setOrigin(startVec);
	end.setOrigin(endVec);

	CW::ClosestConvexResultCallback callback(startVec, endVec);
	callback.m_collisionFilterMask = 2;

	GetEngine().GetPhysicsWorld().ConvexSweepTest
	((const btConvexShape*)m_collision.GetCollisionObject().getCollisionShape(), start, end, callback);
	if (callback.hasHit()) {
		DeleteGO(this, false);
	}
}

CVector3 Bullet::getHitVec() const {
	CVector3 v = m_dir;
	v.Normalize();
	v *= 100;
	v.y = 100;
	return v;
}
