#pragma once

class TpsCamera
{
public:

	void Init(const CVector3& tar, const CVector3& off, const CVector3& up) {
		AddGO(&m_camera);

		SetTarget(tar);
		SetOffset(off);
		SetUp(up);
		m_rot = CVector2::Zero();
		UpdateVector();
		Update();
	}

	//カメラ回転
	void RotationCamera(const CVector2& rot) {
		m_rot += rot;
		if (m_rot.x < -CMath::PI2) { m_rot.x -= -CMath::PI2; }
		if (m_rot.x >  CMath::PI2) { m_rot.x -=  CMath::PI2; }
		if (m_rot.y < -CMath::PI2) { m_rot.y -= -CMath::PI2; }
		if (m_rot.y >  CMath::PI2) { m_rot.y -=  CMath::PI2; }

		UpdateVector();
	}	
	//カメラ位置設定
	void SetTarget(const CVector3& vec) {
		m_target = vec;
	}

	void Update() {
		//カメラ更新
		m_camera.SetPos(m_target + m_ar_offsetPos);
		m_camera.SetTarget(m_target);
		m_camera.SetUp(m_ar_up);
		m_camera.UpdateMatrix();
	}

	GameObj::PerspectiveCamera& GetCamera() {
		return m_camera;
	}
	void SetToMainCamera() {
		SetMainCamera(&m_camera);
	}

	CVector3 GetFront() const{
		return m_ar_offsetPos.GetNorm() * -1.0f;
	}
	CVector3 GetUp() const {
		return m_ar_up.GetNorm();
	}

private:

	//回転後の座標算出
	void UpdateVector() {
		m_ar_offsetPos = m_offsetPos, m_ar_up = m_up;

		CQuaternion cq;
		cq.SetRotation(CVector3::AxisX(), m_rot.y);
		cq.Multiply(m_ar_offsetPos);
		cq.Multiply(m_ar_up);

		cq.SetRotation(CVector3::AxisY(), m_rot.x);
		cq.Multiply(m_ar_offsetPos);
		cq.Multiply(m_ar_up);
	}
	
	void SetOffset(const CVector3& vec) {
		m_offsetPos = vec;
	}
	void SetUp(const CVector3& vec) {
		m_up = vec;
	}

	CVector3 m_offsetPos,m_target,m_up;
	CVector3 m_ar_offsetPos, m_ar_up;
	CVector2 m_rot;
	GameObj::PerspectiveCamera m_camera;
};

