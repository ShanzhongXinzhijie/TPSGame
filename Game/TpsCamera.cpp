#include "stdafx.h"
#include "TpsCamera.h"

void TpsCamera::Init(const CVector3& tar, const CVector3& off, const CVector3& up) {
	AddGO(&m_camera);

	SetTarget(tar);
	SetOffset(off);
	SetUp(up);
	m_rot = CVector2::Zero();
	UpdateVector();
	Update();
}

void TpsCamera::RotationCamera(const CVector2& rot) {
	m_rot += rot;
	if (m_rot.x < -CMath::PI2) { m_rot.x += CMath::PI2; }
	if (m_rot.x > CMath::PI2) { m_rot.x -= CMath::PI2; }
	if (m_rot.y < -CMath::PI / 2.2) { m_rot.y = -CMath::PI / 2.2; }
	if (m_rot.y > CMath::PI / 6) { m_rot.y = CMath::PI / 6; }

	UpdateVector();
}

void TpsCamera::Update() {
	//ÉJÉÅÉâçXêV
	m_camera.SetPos(m_target + m_ar_offsetPos);
	m_camera.SetTarget(m_target);
	m_camera.SetUp(m_ar_up);
	m_camera.UpdateMatrix();
}

void TpsCamera::UpdateVector() {
	m_ar_offsetPos = m_offsetPos, m_ar_up = m_up;

	CQuaternion cq;
	cq.SetRotation(CVector3::AxisX(), m_rot.y);
	cq.Multiply(m_ar_offsetPos);
	cq.Multiply(m_ar_up);

	cq.SetRotation(CVector3::AxisY(), m_rot.x);
	cq.Multiply(m_ar_offsetPos);
	cq.Multiply(m_ar_up);
}