#include "stdafx.h"
#include "TpsCamera.h"

#ifdef SpritScreen
bool TpsCamera::s_isSecond = false;
#endif

TpsCamera::TpsCamera(int pad, const CVector3& tar): padNum(pad), m_target(tar){
	SetTarget(tar);
	SetOffset(CVector3::AxisZ()*distance);
	SetUp(CVector3::Up());
	m_rot = CVector2::Zero();
	UpdateVector();
	Update();
	m_camera.SetFar(8000.0f);
	//メインカメラに設定
#ifdef SpritScreen
	GetCameraList().push_back(&m_camera);
	if (s_isSecond) {
		isSecond = true;
		s_isSecond = false;
	} else {
		s_isSecond = true;
	}
#else
	SetMainCamera(&m_camera);
#endif

	sprite.Init(L"Resource/spriteData/Target.dds");
}

TpsCamera::~TpsCamera() {
	auto& list = GetCameraList();
	for (auto itr = list.begin(), end = list.end(); itr != end; itr++) {
		if ((*itr) == &m_camera) {
			list.erase(itr);
			break;
		}
	}
}

void TpsCamera::RotationCamera(const CVector2& rot) {
	m_rot += rot;
	if (m_rot.x < -CMath::PI2) { m_rot.x += CMath::PI2; }
	if (m_rot.x > CMath::PI2) { m_rot.x -= CMath::PI2; }
	if (m_rot.y < -CMath::PI / 2.1f) { m_rot.y = -CMath::PI / 2.1f; }
	if (m_rot.y > CMath::PI/2.1f) { m_rot.y = CMath::PI/2.1f; }

	UpdateVector();
}

void TpsCamera::moveLR() {
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
}

void TpsCamera::Update() {
	moveLR();
	//カメラ回転
	CVector2 stickMove = Pad(padNum).GetStick(enLR::R);

	//二次関数的な入力にする
	float len = stickMove.Length(); len = pow(len, 2.0f); //len = pow(25.0f, len)/25.0f; //len -= 1.0f;
	stickMove.Normalize();
	stickMove *= len;
	if (slow) {
		stickMove *= 0.3;
	}

	stickMove = stickMove * 0.05f;
	if (backTurnRad > 0.0f) {
		float rad = CMath::PI * GetDeltaTimeSec() / backTurnSpeed;
		if (backTurnRad < rad) {
			stickMove.x += backTurnRad;
			backTurnRad = 0.0f;
		} else {
			stickMove.x += rad;
			backTurnRad -= rad;
		}
	}
	RotationCamera(stickMove);

	//カメラ更新
	CVector3 upCamera = {0,0,0};
	if (!upIsTarget) {
		upCamera = GetUp()*up*0.5;
	}

	CVector3 springPower = (m_target - m_springTarget);
	m_springTarget += springPower*0.3;

	m_camera.SetPos(m_springTarget + m_ar_offsetPos + GetRight()*side + upCamera);
	m_camera.SetTarget(m_springTarget+ GetRight()*side + upCamera);
	m_camera.SetUp(m_ar_up);
	m_camera.UpdateMatrix();
}

void TpsCamera::PostRender() {
#ifdef SpritScreen
	if (isSecond) {
		sprite.Draw({ 0.75f, 0.5f }, CVector2::One(), { 0.5,0.5 });
	} else {
		sprite.Draw({ 0.25f, 0.5f }, CVector2::One(), { 0.5,0.5 });
    }
#else
	sprite.Draw({0.5f, 0.5f}, CVector2::One(), { 0.5,0.5 });
#endif
}

void TpsCamera::UpdateVector() {
	m_ar_offsetPos = m_offsetPos, m_ar_up = m_up;

	CQuaternion cq;
	cq.SetRotation(CVector3::AxisX(), m_rot.y);
	cq.Multiply(m_ar_offsetPos);
	cq.Multiply(m_ar_up);

	cq.SetRotation(CVector3::AxisY(), m_rot.x);
	cq.Multiply(m_ar_offsetPos);
	cq.Multiply(m_ar_up);
}

btVector3 toBtV(CVector3 v) {
	return btVector3(v.x, v.y, v.z);
}

CVector3 toCV(btVector3 v) {
	return CVector3(v.x(), v.y(), v.z());
}

CVector3 TpsCamera::getLook() const {
	return (m_camera.GetTarget() - m_camera.GetPos()) / distance;
}

CVector3 TpsCamera::getLook(const CVector3& myPos) const{
	using bw = btCollisionWorld;
	CVector3 lookVec = getLook();
	btVector3 startPos = toBtV(m_camera.GetPos());
	btVector3 targetPos = toBtV(m_camera.GetPos() + lookVec * 4000);

	bw::ClosestRayResultCallback callback(startPos, targetPos);

	GetPhysicsWorld().RayTest(startPos, targetPos, callback);

	if (!callback.hasHit()) {
		return lookVec;
	}

	CVector3 finalLook = toCV(callback.m_hitPointWorld) - myPos;

	finalLook.Normalize();
	return finalLook;
}

