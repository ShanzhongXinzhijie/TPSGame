#pragma once

class TpsCamera
{
public:

	void Init(const CVector3& tar, const CVector3& off, const CVector3& up);

	//�J������]
	void RotationCamera(const CVector2& rot);
	//�J�����ʒu�ݒ�
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

	//��]��̍��W�Z�o
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

