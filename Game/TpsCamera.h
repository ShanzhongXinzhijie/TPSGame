#pragma once

class TpsCamera : public IGameObject {
public:
	TpsCamera(int pad, const CVector3& tar, float distance);

	void Update() override;

	//カメラ位置設定
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
	//カメラ回転
	void RotationCamera(const CVector2& rot);
	//回転後の座標算出
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

	const float rightSide = 90.0f; //右よりの場合の横移動
	const float up = 100.0f;    //カメラ位置　上下
	const float distance = 100.0f; //カメラの距離
	float side = rightSide; //カメラの左右
	bool isRight = true; //カメラが右よりならtrue

	const float backTurnSpeed = 0.3f; //振り向きにかかる時間
	float backTurnRad = 0.0f; //振り向き残りの角度

	const int padNum;
};

