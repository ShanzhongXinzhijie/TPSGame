#include "stdafx.h"
#include "CPlayer.h"
#include "CPlayerCamera.h"

CPlayer::CPlayer() {
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
		}
	}
}

void CPlayer::Move() {
	CVector2 movement = action.getMovement();

	//�_�b�V������
	float speed = moveSpeed;
	bool dash = false;
	if (action.isDash()) {
		speed = moveSpeed * dashMul;
		dash = true;
	}

	//�󒆂ɂ���ꍇ��C��R�����A����͌����ɂ����Ȃ�B(���C�͋�C��R�Ƃ��Čv�Z����)
	float airResist = friction;
	if (!charaCon.IsOnGround() || action.isJump()) {
		speed = moveSpeed * 0.2f;
		airResist *= 0.1f;
	}

	//x�ړ�
	velocity.x += movement.x * speed;

	//z�ړ�
	velocity.z += movement.y * speed;

	//��C��R�B
	{
		char sign = 1;
		if (velocity.x < 0) {
			sign = -1;
		}
		velocity.x -= airResist * velocity.x * GetDeltaTimeSec();
		if (velocity.x * sign < 0) {
			velocity.x = 0;
		}

		sign = 1;
		if (velocity.z < 0) {
			sign = -1;
		}
		velocity.z -= airResist * velocity.z * GetDeltaTimeSec();
		if (velocity.z * sign < 0) {
			velocity.z = 0;
		}
	}

	//�ʒu�X�V
	m_pos = charaCon.Execute(velocity);

	//�A�j���[�V�����Ɖ�]
	if (movement.x != 0 && movement.y != 0) {
		m_rot = CQuaternion::GetRotation(CVector3::AxisY(), atan2f(movement.x, movement.y));

		if (dash) {
			m_model.GetAnimCon().Play(anim_run, animInterpolateSec);
		} else {
			m_model.GetAnimCon().Play(anim_walk, animInterpolateSec);
		}
	} else {
		m_model.GetAnimCon().Play(anim_idle, animInterpolateSec);
	}

	//���f���X�V
	m_model.SetPRS(m_pos, m_rot, { 1.0f,1.0f ,1.0f });

}