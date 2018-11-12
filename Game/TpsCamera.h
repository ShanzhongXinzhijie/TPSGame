#pragma once

class TpsCamera
{
public:

	void Init(const CVector3& tar, const CVector3& off, const CVector3& up);

	//カメラ回転
	void RotationCamera(const CVector2& rot);
	//カメラ位置設定
	void SetTarget(const CVector3& vec) {
		m_target = vec;
	}

	void Update();

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
	void UpdateVector();
	
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

