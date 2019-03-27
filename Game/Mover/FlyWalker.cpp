#include "stdafx.h"
#include "FlyWalker.h"
#include "SE_Util.h"

FlyWalker::FlyWalker() {
}


FlyWalker::~FlyWalker() {
	if (se != nullptr) {
		se->Stop();
	}
}

void FlyWalker::fly(bool isFly, const CVector3 & v, const CVector2& move, float power) {
	if (isFly) {
		if (flyTimer > 0.0f && !m_isRest) {
			playSE(L"Resource/sound/SE_fly.wav", GetPosition());//飛び立つ音
			se = playSE(L"Resource/sound/SE_flying.wav", GetPosition(), 500.0f, true);//飛行中の風の音

			//視線方向にパワー分だけ加速
			flyPower = power;
			CVector3 v2 = v;
			v2.y = upDown = 0;//水平方向にする
			velocity = v2 * flyPower;
			flying = true;

			springRot = Walker::getRotation();//追従すべき回転を設定。
		}
	} else if (flying) {
		if (se != nullptr) {
			se->SetPos(GetPosition());//風切り音の発生位置を自分の位置に更新
		}
		//視線単位ベクトルからy成分を除去
		CVector3 v2 = v;
		v2.y = 0;
		v2.Normalize();

		//上下の方向を滑らかに変更
		upDown += (move.y*2.4f - upDown) * GetDeltaTimeSec() * 2;

		v2.y = upDown;
		v2.Normalize();
		velocity = v2 * flyPower * GetDeltaTimeSec();

		if (flyPower <= 0) {//時間切れなら飛行停止
			flyStop();
		}
	}
}

void FlyWalker::flyStop() {
	velocity *= 0.5f;
	flying = false;
	coolTimer = c_coolTimer;
	if (se) {//風切り音を停止
		se->Stop();
		se = nullptr;
	}
}

void FlyWalker::rest() {
	flyTimer = 0.0f;
	m_restEfkTimer = c_restEfkTimer;
	m_isRest = true;
	CVector3 pos = GetPosition();
	pos.y += 50.0f;
	restEffect = new GameObj::Suicider::CEffekseer(L"Resource/effect/flyStop.efk", 1.0f, pos);
	restEffect->SetIsSuicide(false);
	playSE(L"Resource/sound/SE_flyStop.wav", GetPosition());
}
void FlyWalker::restStop() {
	flyTimer = c_flyTimer;
	if (isFlyRest()) {
		coolTimer = 0.0f;
	}
	m_isRest = false;
}

CQuaternion FlyWalker::getRotation() const {
	return springRot;
}

void FlyWalker::Update() {
	hitWall = false;
	if (flying) {
		{//バネ回転
			CQuaternion rot;
			rot.SetRotation(CVector3::AxisY(), atan2f(velocity.x, velocity.z));

			float xz = velocity.x*velocity.x + velocity.z*velocity.z;
			xz = sqrt(xz);

			rot.Multiply(CQuaternion::GetRotation(CVector3::AxisX(), atan2f(-velocity.y, xz) + (CMath::PI * 0.5f)));

			springRot.Slerp(0.1f, springRot, rot);
		}

		//飛行可能時間減少
		flyTimer -= GetDeltaTimeSec();
		if (flyTimer <= 0.0f) {
			if (m_isLocalUser) {
				rest();
			}
		}

		//地面に立つか時間切れでストップ
		if (IsOnGround() || flyTimer == 0.0f) {
			flyStop();
		}

		//壁に衝突した場合
		btVector3 from = GetPosition();
		CVector3 vec = velocity;
		vec.Normalize();
		btVector3 to = from + vec * 100.0f;
		btCollisionWorld::ClosestRayResultCallback callback(from, to);
		callback.m_collisionFilterMask = 2;
		GetPhysicsWorld().RayTest(from, to, callback);
		if (callback.hasHit()) {
			flyStop();
			btVector3 normal = callback.m_hitNormalWorld;
			float x = sqrt(normal.getX()*normal.getX() + normal.getZ()*normal.getZ());
			float tan = fabsf(normal.getY() / x + 0.00001f);
			if (tan < 0.5f) {
				//壁に衝突した場合跳ね返りがある
				hitWall = true;
				CVector3 dir;
				dir.Set(normal);
				dir.y = 0.0f;
				dir += CVector3::AxisY();
				velocity = dir * 600.0f;
			}
		}

	} else {
		//回転をspringRotに追従させる
		springRot.Slerp(0.1f, springRot, Walker::getRotation());

		CVector3 pos = GetPosition();
		pos.y += 50.0f;

		if (isFlyRest()) {//rest中のエフェクト
			m_restEfkTimer -= GetDeltaTimeSec();
			if (m_restEfkTimer <= 0.0f) {
				m_restEfkTimer = c_restEfkTimer;
				restEffect = new GameObj::Suicider::CEffekseer(L"Resource/effect/flyRest.efk", 1.0f, pos);
				restEffect->SetIsSuicide(false);
				playSE(L"Resource/sound/SE_flyRest.wav", GetPosition());
			}
		}

		if (restEffect) {//restEffectの位置を更新する。
			if (restEffect->IsPlay()) {
				restEffect->SetPos(pos);
			} else {
				delete restEffect;
				restEffect = nullptr;
			}
		}
		if (recoverEffect) {//recoverEffectの位置を更新する
			if (recoverEffect->IsPlay()) {
				recoverEffect->SetPos(pos);
			} else {
				delete recoverEffect;
				recoverEffect = nullptr;
			}
		}

		//飛行可能時間の回復
		if (coolTimer > 0.0f) {
			//まずはクールタイムを減らす
			coolTimer -= GetDeltaTimeSec();
		}else if (flyTimer < c_flyTimer) {
			//それが終わってから飛行タイマーが回復する。
			flyTimer += GetDeltaTimeSec();
			if (flyTimer >= c_flyTimer) {
				restStop();
			} else if (isFlyRest() && !recoverEffect &&(c_flyTimer-flyTimer) < c_recoverEfkTime) {
				recoverEffect = new GameObj::Suicider::CEffekseer(L"Resource/effect/flyRecover.efk", 1.0f, pos);
				recoverEffect->SetIsSuicide(false);
				playSE(L"Resource/sound/SE_flyRecover.wav", GetPosition());
			}
		}
	}
	Walker::Update();
}
