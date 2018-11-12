#include "stdafx.h"
#include "CPlayerCamera.h"

CPlayerCamera::CPlayerCamera(int pad) : padNum(pad){

}

bool CPlayerCamera::Start() {
	//TPSカメラの初期化
	m_camera.Init(CVector3::Zero(), CVector3::AxisZ()*100.0f, CVector3::Up());
	m_camera.GetCamera().SetFar(2000.0f);
	//メインカメラに設定
	m_camera.SetToMainCamera();

	m_camera.RotationCamera({ 0.0f,0.0f });


	return true;
}

void CPlayerCamera::Update() {
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
	//スティックでカメラ回転
	CVector2 stickMove = Pad(padNum).GetStick(enLR::R);
	stickMove = stickMove * 0.05f;
	m_camera.RotationCamera(stickMove);
	//カメラ更新
	m_camera.Update();
};

void CPlayerCamera::SetPos(CVector3 v) {
	v += GetRight() * side;
	v.y += up;
	m_camera.SetTarget(v);
}