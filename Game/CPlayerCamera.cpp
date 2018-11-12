#include "stdafx.h"
#include "CPlayerCamera.h"

CPlayerCamera::CPlayerCamera(int pad) : padNum(pad){

}

bool CPlayerCamera::Start() {
	//TPS�J�����̏�����
	m_camera.Init(CVector3::Zero(), CVector3::AxisZ()*100.0f, CVector3::Up());
	m_camera.GetCamera().SetFar(2000.0f);
	//���C���J�����ɐݒ�
	m_camera.SetToMainCamera();

	m_camera.RotationCamera({ 0.0f,0.0f });


	return true;
}

void CPlayerCamera::Update() {
	//�J�����̍��E�ړ���0.2�b�ōs��
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
	//�X�e�B�b�N�ŃJ������]
	CVector2 stickMove = Pad(padNum).GetStick(enLR::R);
	stickMove = stickMove * 0.05f;
	m_camera.RotationCamera(stickMove);
	//�J�����X�V
	m_camera.Update();
};

void CPlayerCamera::SetPos(CVector3 v) {
	v += GetRight() * side;
	v.y += up;
	m_camera.SetTarget(v);
}