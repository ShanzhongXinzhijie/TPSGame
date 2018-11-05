#pragma once

#include"TpsCamera.h"

class CPlayerCamera : public IGameObject {
public:
	bool Start()override {
		//TPSカメラの初期化
		m_camera.Init(CVector3::Zero(), CVector3::AxisZ()*200.0f, CVector3::Up());
		m_camera.GetCamera().SetFar(2000.0f);
		//メインカメラに設定
		m_camera.SetToMainCamera();

		//マウスカーソルを中央に固定
		GetMouseCursorManager().SetLockMouseCursor(true);
		//マウスカーソルを非表示
		GetMouseCursorManager().SetShowMouseCursor(false);

		m_camera.RotationCamera({ 0.0f,CMath::PI*-0.5f });


		return true;
	}
	void Update()override {
		if (!m_lock) {
			//マウスカーソルの動きに連動してカメラ回転
			CVector2 mousemove = GetMouseCursorManager().GetMouseMove();
			mousemove.x *= m_sensi.x; mousemove.y *= m_sensi.y;
			m_camera.RotationCamera(mousemove);
		}

		if (GetAsyncKeyState('R')) {
			m_camera.RotationCamera({ 0.31f,0.0f });
		}
		if (GetAsyncKeyState('Q')) {
			m_camera.RotationCamera({ -0.31f,0.0f });
		}

		if (GetAsyncKeyState('O')) {
			//マウスカーソルを中央に固定
			GetMouseCursorManager().SetLockMouseCursor(true);
			//マウスカーソルを非表示
			GetMouseCursorManager().SetShowMouseCursor(false);
			m_lock = false;
		}
		if (GetAsyncKeyState('P')) {
			//マウスカーソルを中央に固定
			GetMouseCursorManager().SetLockMouseCursor(false);
			//マウスカーソルを非表示
			GetMouseCursorManager().SetShowMouseCursor(true);
			m_lock = true;
		}

		//カメラ更新
		m_camera.Update();
	};

	void SetPos(const CVector3& v) {
		m_camera.SetTarget(v);
	}

	CVector3 GetFront()const {
		return m_camera.GetFront();
	}
	CVector3 GetUp()const {
		return m_camera.GetUp();
	}
	CVector3 GetLeft()const {
		return CVector3::GetCross(m_camera.GetFront(), m_camera.GetUp());
	}

private:
	TpsCamera m_camera;
	CVector2 m_sensi = { 4.0f*(1.0f/1280.0f),-4.0f*(1.0f / 1280.0f) };
	bool m_lock = false;
};

