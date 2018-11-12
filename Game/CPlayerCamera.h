#pragma once

#include"TpsCamera.h"

class CPlayerCamera : public IGameObject {
public:
	CPlayerCamera(int pad);

	bool Start() override;

	void Update() override;

	void SetPos(CVector3 v);

	void setLeft() {
		isRight = false;
	}

	void setRigth() {
		isRight = true;
	}

	CVector3 GetFront()const {
		return m_camera.GetFront();
	}
	CVector3 GetUp()const {
		return m_camera.GetUp();
	}
	CVector3 GetRight()const {
		return CVector3::GetCross(m_camera.GetFront(), m_camera.GetUp()) * -1;
	}

private:
	TpsCamera m_camera;
	const float rightSide = 90.0f; //右よりの場合の横移動
	const float up = 100.0f;    //カメラ位置　上下
	float side = rightSide; //カメラの左右
	bool isRight = true; //カメラが右よりならtrue

	const int padNum;
};

