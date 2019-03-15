#pragma once

class TpsCamera : public IGameObject {
public:
	TpsCamera(int pad, const CVector3& tar);
	~TpsCamera();

	void BackTurn() {
		if (backTurnRad <= 0.0f) {
			backTurnRad = CMath::PI;
		}
	}

	CVector3 GetFront() const {
		return m_ar_offsetPos.GetNorm() * -1.0f;
	}
	CVector3 GetRight()const {
		return CVector3::GetCross(GetFront(), GetUp()) * -1;
	}
	CVector3 GetUp() const {
		return m_ar_up.GetNorm();
	}

	CVector3 getLook() const;

	CVector3 getLook(const CVector3& myPos) const;


	void PostRender() override;

	void PreUpdate() override;

	//�J�����ʒu�ݒ�
	void SetTarget(const CVector3& vec) {
		m_target = vec;
	}

	void SetToMainCamera() {
		SetMainCamera(&m_camera);
	}

	void setLeft() {
		isRight = false;
	}

	void setRight() {
		isRight = true;
	}

	bool getLeft()const { return !isRight; }

	void setRot(const CVector2& rot) {
		Rstick = rot;
	}

	void setSlow(bool slow) {
		this->slow = slow;
	}

	void setSpring(float spring) {
		this->spring = spring;
	}

	void setZoomScale(float zoomScale) {
		zoom = 1.0f / zoomScale;
	}

private:
	//�J������]
	void RotationCamera(const CVector2& rot);
	//��]��̍��W�Z�o
	void UpdateVector(const CVector2& rot);

	void moveLR();

	void SetOffset(const CVector3& vec) {
		m_offsetPos = vec;
	}
	void SetUp(const CVector3& vec) {
		m_up = vec;
	}

	CSprite sprite;

	CVector3 m_offsetPos, m_target, m_up;
	CVector3 m_ar_offsetPos, m_ar_up;
	CVector2 m_rot = {0.0f, 0.0f};
	CVector2 springRot = m_rot;
	float spring = 0.2f;
	GameObj::PerspectiveCamera m_camera;


	CVector2 Rstick = {}; //�X�e�B�b�N����

	static constexpr float viewAngle = 3.14f*0.5f; //����p
	float zoom = 1.0f; //����p�̏k����

	CVector3 m_springTarget = m_target; //�΂˃J�����̈ʒu

	//�E���̏ꍇ�̉��ړ�
	static constexpr float rightSide
#ifdef SpritScreen
		= 50.0f;
#else
		= 80.0f;
#endif

	const float distance = 100.0f; //�J�����̋���
	float side = rightSide; //�J�����̍��E
	bool isRight = true; //�J�������E���Ȃ�true

	static constexpr float backTurnSpeed = 0.3f; //�U������ɂ����鎞��
	float backTurnRad = 0.0f; //�U������c��̊p�x

	const int padNum;

	bool slow = false;

#ifdef SpritScreen
	static bool s_isSecond;
	bool isSecond = false;
#endif
};

