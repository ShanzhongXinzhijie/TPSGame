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

	//カメラ位置設定
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
	//カメラ回転
	void RotationCamera(const CVector2& rot);
	//回転後の座標算出
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


	CVector2 Rstick = {}; //スティック入力

	static constexpr float viewAngle = 3.14f*0.5f; //視野角
	float zoom = 1.0f; //視野角の縮小率

	CVector3 m_springTarget = m_target; //ばねカメラの位置

	//右よりの場合の横移動
	static constexpr float rightSide
#ifdef SpritScreen
		= 50.0f;
#else
		= 80.0f;
#endif

	const float distance = 100.0f; //カメラの距離
	float side = rightSide; //カメラの左右
	bool isRight = true; //カメラが右よりならtrue

	static constexpr float backTurnSpeed = 0.3f; //振り向きにかかる時間
	float backTurnRad = 0.0f; //振り向き残りの角度

	const int padNum;

	bool slow = false;

#ifdef SpritScreen
	static bool s_isSecond;
	bool isSecond = false;
#endif
};

