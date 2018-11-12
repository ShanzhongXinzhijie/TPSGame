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
	//�A�b�v�f�[�g�I�����A�A�N�V����������������B
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
			//x�ړ�
			velocity.x += movement.x * moveSpeed *3;
			//z�ړ�
			velocity.z += movement.y * moveSpeed *3;
		}
	}
}

void CPlayer::Move() {
	CVector2 movement = action.getMovement();

	CVector3 moveVec;

	//x�ړ�
	moveVec.x += movement.x * moveSpeed;

	//z�ړ�
	moveVec.z += movement.y * moveSpeed;

	bool dash = false;
	if (action.isDash()) {
		moveVec *= dashMul;
		dash = true;
	}
	//�󒆂ɂ���ꍇ�ړ����x����
	if (!charaCon.IsOnGround()) {
		moveVec *= 0;
	} else {
		//�n�ʂɂ���Ƃ��͖��C�Ŋ����������Ă���
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