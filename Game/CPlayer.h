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
	const float animInterpolateSec = 0.2f;        //アニメーション補間時間

	CVector3 m_pos = { 150.0f, 0.0f, 150.0f };
	CQuaternion m_rot;

	const float moveSpeed = 80.0f;               //移動速度
	const float dashMul = 2.0f;                   //ダッシュ倍率
	const float friction = 10.0f;               //摩擦
	const float gravity = 980.0f;                   //重力加速度
	const float jumpPower = 600.0f;                //ジャンプ力
	CVector3 velocity = { 0.0f, 0.0f, 0.0f };	 //速度
	CCharacterController charaCon;
	ActionSender action;     //プレイヤーの操作が入っている
};
