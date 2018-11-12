#include "stdafx.h"
#include "CPlayer.h"
#include "CPlayerCamera.h"

CPlayer::CPlayer(){
}

bool CPlayer::Start() {
	m_animationClips[anim_run].Load(L"Resource/animData/run.tka", true);
	m_animationClips[anim_walk].Load(L"Resource/animData/walk.tka", true);
	m_animationClips[anim_idle].Load(L"Resource/animData/idle.tka", true);

	m_model.Init(L"Resource/modelData/unityChan.cmo", m_animationClips, anim_num, enFbxUpAxisY);

	charaCon.Init(30.0f, 80.0f, m_pos);

	return true;
};

void CPlayer::Update() {
	GravityAndJump();
	Move();
	//アップデート終了時、アクションを初期化する。
	action = ActionSender();
}

CVector3 CPlayer::getPosition() {
	return m_pos;
}

void CPlayer::sendAction(const ActionSender& actionPal) {
	action = actionPal;
}

void CPlayer::GravityAndJump() {
	if (!charaCon.IsOnGround()) {
		velocity.y -= gravity * GetDeltaTimeSec();
	} else {
		if (velocity.y < 0) {
			velocity.y = 0;
		}
		if (action.isJump()) {
			velocity.y = jumpPower;
			CVector2 movement = action.getMovement();
			//x移動
			velocity.x += movement.x * moveSpeed *3;
			//z移動
			velocity.z += movement.y * moveSpeed *3;
		}
	}
}

void CPlayer::Move() {
	CVector2 movement = action.getMovement();

	CVector3 moveVec;

	//x移動
	moveVec.x += movement.x * moveSpeed;

	//z移動
	moveVec.z += movement.y * moveSpeed;

	bool dash = false;
	if (action.isDash()) {
		moveVec *= dashMul;
		dash = true;
	}
	//空中にいる場合移動速度減少
	if (!charaCon.IsOnGround()) {
		moveVec *= 0;
	} else {
		//地面にいるときは摩擦で慣性が減っていく
		char sign = 1;
		if (velocity.x < 0) {
			sign = -1;
		}
		velocity.x -= 400 * sign;
		if (velocity.x * sign <= 0) {
			velocity.x = 0;
		}

		if (velocity.z < 0) {
			sign = -1;
		}
		velocity.z -= 400 * sign;
		if (velocity.z * sign <= 0) {
			velocity.z = 0;
		}
	}

	moveVec += velocity;

	m_pos = charaCon.Execute(moveVec);

	if (moveVec.x != 0 && moveVec.z != 0) {
		m_rot = CQuaternion::GetRotation(CVector3::AxisY(), atan2f(moveVec.x, moveVec.z));

		if (dash) {
			m_model.GetAnimCon().Play(anim_run, animInterpolateSec);
		} else {
			m_model.GetAnimCon().Play(anim_walk, animInterpolateSec);
		}
	} else {
		m_model.GetAnimCon().Play(anim_idle, animInterpolateSec);
	}

	m_model.SetPRS(m_pos, m_rot, { 1.0f,1.0f ,1.0f });

}