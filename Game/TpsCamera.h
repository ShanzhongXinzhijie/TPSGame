#pragma once

class TpsCamera : public IGameObject {
public:
	TpsCamera(int pad, const CVector3& tar, float distance);

	void Update() override;

	//�J�����ʒu�ݒ�
	void SetTarget(const CVector3& vec) {
		m_target = vec;
		m_target.y += up;
	}

	void SetToMainCamera() {
		SetMainCamera(&m_camera);
	}

	void setLeft() {
		isRight = false;
	}

	void setRigth() {
		isRight = true;
	}

	CVector3 GetFront() const {
		return m_ar_offsetPos.GetNorm() * -1.0f;
	}
	CVector3 GetUp() const {
		return m_ar_up.GetNorm();
	}
	CVector3 GetRight()const {
		return CVector3::GetCross(GetFront(), GetUp()) * -1;
	}

	void BackTurn() {
		if (backTurnRad <= 0.0f) {
			backTurnRad = CMath::PI;
		}
	}

	CVector3 getLook() const;

private:
	//�J������]
	void RotationCamera(const CVector2& rot);
	//��]��̍��W�Z�o
	void UpdateVector();

	void moveLR();

	void SetOffset(const CVector3& vec) {
		m_offsetPos = vec;
	}
	void SetUp(const CVector3& vec) {
		m_up = vec;
	}

	CVector3 m_offsetPos, m_target, m_up;
	CVector3 m_ar_offsetPos, m_ar_up;
	CVector2 m_rot = {0.0f, 0.0f};
	GameObj::PerspectiveCamera m_camera;

	const float rightSide = 90.0f; //�E���̏ꍇ�̉��ړ�
	const float up = 100.0f;    //�J�����ʒu�@�㉺
	const float distance = 100.0f; //�J�����̋���
	float side = rightSide; //�J�����̍��E
	bool isRight = true; //�J�������E���Ȃ�true

	const float backTurnSpeed = 0.3f; //�U������ɂ����鎞��
	float backTurnRad = 0.0f; //�U������c��̊p�x

	const int padNum;
};

