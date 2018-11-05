#pragma once

#include "CPlayerCamera.h"

#include "DemolisherWeapon/physics/PhysicsStaticObject.h"
#include "DemolisherWeapon/physics/CapsuleCollider.h"
#include "DemolisherWeapon/physics/RigidBody.h"

class CPlayer : public IGameObject {
public:
	bool Start()override {

		m_animationClips[0].Load(L"Resource/animData/run.tka",true);
		m_animationClips[1].Load(L"Resource/animData/walk.tka");

		m_model.Init(L"Resource/modelData/unityChan.cmo", m_animationClips, 2, enFbxUpAxisY);		
		
		//コリジョン作成。
		m_collider.Create(50.0f, 50.f);
		btVector3 inertia(0, 0, 0);
		m_collider.GetBody()->calculateLocalInertia(1.0f, inertia);

		//剛体を初期化。
		RigidBodyInfo rbInfo;
		rbInfo.collider = &m_collider;
		rbInfo.mass = 1.0f;
		m_rigidBody.Create(rbInfo);

		btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
		//剛体の位置を更新。
		trans.setOrigin(btVector3(m_pos.x, m_pos.y, m_pos.z));
		//m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_Character);
		//m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
		GetEngine().GetPhysicsWorld().AddRigidBody(m_rigidBody);

		return true;
	};

	void Update()override {

		/*if (!GetAsyncKeyState('V')) {
			btTransform trans;
			m_rigidBody.GetBody()->getMotionState()->getWorldTransform(trans);
			m_pos.x = trans.getOrigin().x();
			m_pos.y = trans.getOrigin().y();
			m_pos.z = trans.getOrigin().z();
		}*/
		if (GetAsyncKeyState('Z')) {
			m_rot.Multiply(CQuaternion::GetRotationDeg(CVector3::AxisY(), 4.0f));
		}
		if (GetAsyncKeyState('X')) {
			m_rot.Multiply(CQuaternion::GetRotationDeg(CVector3::AxisY(), -4.0f));
		}

		if (GetAsyncKeyState('W')) {
			m_pos += m_cam.GetFront()*40.0f;
		}
		if (GetAsyncKeyState('S')) {
			m_pos -= m_cam.GetFront()*40.0f;
		}
		if (GetAsyncKeyState('A')) {
			m_pos += m_cam.GetLeft()*40.0f;
		}
		if (GetAsyncKeyState('D')) {
			m_pos -= m_cam.GetLeft()*40.0f;
		}
		if (GetAsyncKeyState(VK_SPACE)) {
			m_pos += m_cam.GetUp()*40.0f;
		}
		if (GetAsyncKeyState('C')) {
			m_pos -= m_cam.GetUp()*4.0f;
		}
		if (GetAsyncKeyState('M')) {
			m_isCamMove = true; m_model.GetAnimCon().Play(1,0.3f); m_model.GetAnimCon().SetSpeed(0.5f);
		}
		if (GetAsyncKeyState('N')) {
			m_isCamMove = false; m_model.GetAnimCon().Play(0,0.3f); m_model.GetAnimCon().SetSpeed(2.0f);
		}

		btRigidBody* btBody = m_rigidBody.GetBody();
		//剛体を動かす。
		//btBody->setActivationState(DISABLE_DEACTIVATION);
		//btTransform& trans = btBody->getWorldTransform();
		//剛体の位置を更新。
		btBody->getWorldTransform().setOrigin(btVector3(m_pos.x, m_pos.y, m_pos.z));

		//btTransform trans;
		//m_rigidBody.GetBody()->getMotionState()->setWorldTransform(trans);

		m_model.SetPRS(m_pos, m_rot, { 1.0f,1.0f ,1.0f });

		if (m_isCamMove)m_cam.SetPos(m_pos);
	};

private:
	GameObj::CSkinModelRender m_model;
	AnimationClip m_animationClips[2];

	CVector3 m_pos = { 0.0f, 0.0f, 0.0f };
	CQuaternion m_rot;
	bool m_isCamMove = true;
	CPlayerCamera m_cam;

	CapsuleCollider		m_collider;
	RigidBody			m_rigidBody;
};
