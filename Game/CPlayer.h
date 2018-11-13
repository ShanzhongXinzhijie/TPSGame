#pragma once

#include "DemolisherWeapon/physics/character/CCharacterController.h"
#include "ActionSender.h"

class CPlayerCamera;

class CPlayer : public IGameObject {
public:
	CPlayer();

	bool Start() override;
	virtual void Update() override;

	CVector3 getPosition();

	void sendAction(const ActionSender& action);

private:
	void GravityAndJump();
	void Move();

	GameObj::CSkinModelRender m_model;
	enum {
		anim_run,
		anim_walk,
		anim_idle,
		anim_num,
	};
	AnimationClip m_animationClips[anim_num];
	const float animInterpolateSec = 0.2f;        //�A�j���[�V������Ԏ���

	CVector3 m_pos = { 150.0f, 0.0f, 150.0f };
	CQuaternion m_rot;

	const float moveSpeed = 80.0f;               //�ړ����x
	const float dashMul = 2.0f;                   //�_�b�V���{��
	const float friction = 10.0f;               //���C
	const float gravity = 980.0f;                   //�d�͉����x
	const float jumpPower = 600.0f;                //�W�����v��
	CVector3 velocity = { 0.0f, 0.0f, 0.0f };	 //���x
	CCharacterController charaCon;
	ActionSender action;     //�v���C���[�̑��삪�����Ă���
};
