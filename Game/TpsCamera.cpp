#include "stdafx.h"
#include "TpsCamera.h"

TpsCamera::TpsCamera(int pad, const CVector3& tar, float distance): padNum(pad){
	SetTarget(tar);
	SetOffset(CVector3::AxisZ()*distance);
	SetUp(CVector3::Up());
	m_rot = CVector2::Zero();
	UpdateVector();
	Update();
	m_camera.SetFar(3000.0f);
	//メインカメラに設定
#ifdef SpritScreen
	GetCameraList().push_back(&m_camera);
#else
	SetMainCamera(&m_camera);
#endif
}

TpsCamera::~TpsCamera() {
	auto& list = GetCameraList();
	for (auto itr = list.begin(), end = list.end(); itr != end; itr++) {
		if ((*itr) == &m_camera) {
			list.erase(itr);
			break;
		}
	}
}

void TpsCamera::RotationCamera(const CVector2& rot) {
	float speed = 1;
	if (slow) {
		speed = 0.3;
	}
	m_rot += rot * speed;
	if (m_rot.x < -CMath::PI2) { m_rot.x += CMath::PI2; }
	if (m_rot.x > CMath::PI2) { m_rot.x -= CMath::PI2; }
	if (m_rot.y < -CMath::PI / 2.1) { m_rot.y = -CMath::PI / 2.1; }
	if (m_rot.y > CMath::PI/2.1) { m_rot.y = CMath::PI/2.1; }

	UpdateVector();
}

void TpsCamera::moveLR() {
	//カメラの左右移動を0.2秒で行う
	if (isRight) {
		if (side < rightSide) {
			side += GetDeltaTimeSec() * (rightSide * 10);
			if (side > rightSide) {
				side = rightSide;
			}
		}
	} else {
		if (side > -rightSide) {
			side -= GetDeltaTimeSec() * (rightSide * 10);
			if (side < -rightSide) {
				side = -rightSide;
			}
		}
	}
}

void TpsCamera::Update() {
	moveLR();
	//カメラ回転
	CVector2 stickMove = Pad(padNum).GetStick(enLR::R);
	stickMove = stickMove * 0.05f;
	if (backTurnRad > 0.0f) {
		float rad = CMath::PI * GetDeltaTimeSec() / backTurnSpeed;
		if (backTurnRad < rad) {
			stickMove.x += backTurnRad;
			backTurnRad = 0.0f;
		} else {
			stickMove.x += rad;
			backTurnRad -= rad;
		}
	}
	RotationCamera(stickMove);

	//カメラ更新
	m_camera.SetPos(m_target + m_ar_offsetPos + GetRight()*side);
	m_camera.SetTarget(m_target+ GetRight()*side);
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

CVector3 TpsCamera::getLook() const{
	CVector3 look = m_camera.GetTarget() - m_camera.GetPos();
	look.Normalize();
	return look;
}