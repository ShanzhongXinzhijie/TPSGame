#pragma once

#include "DemolisherWeapon/physics/character/CCharacterController.h"
#include "ActionSender.h"

class CPlayerCamera;

class CPlayer : public IGameObject {
public:
	CPlayer(int playerNum ,const CVector3& positon);
	virtual ~CPlayer();

	bool Start() override;
	virtual void Update() override;

	CVector3 getPosition();

	void sendAction(const ActionSender& action);

	bool BatHit(int num, CVector3 dir);

private:
	void GravityAndJump();
	void Move();
	void Turn();
	void Shot();

	GameObj::CSkinModelRender m_model;
	enum {
		anim_run,
		anim_walk,
		anim_idle,
		anim_num,
	};
	AnimationClip m_animationClips[anim_num];
	static constexpr float animInterpolateSec = 0.2f;        //アニメーション補間時間

	unsigned short m_hp = 10;

	const int playerNum;  //識別番号

	CVector3 m_pos;
	CQuaternion m_rot;
	float radian = 0.0f; //回転量

	//銃のクールタイム
	static constexpr float coolTime = 0.1f;
	float shotCoolTime = coolTime;

	//物理系
	static constexpr float moveSpeed = 80.0f;              //移動速度
	static constexpr float dashMul = 2.0f;	             //ダッシュ倍率
	static constexpr float friction = 10.0f;	             //摩擦
	static constexpr float gravity = 980.0f;	             //重力加速度
	static constexpr float jumpPower = 600.0f;             //ジャンプ力

	CVector3 velocity = { 0.0f, 0.0f, 0.0f };	 //速度
	CCharacterController charaCon;
	ActionSender action;     //プレイヤーの操作が入っている

	SuicideObj::CCollisionObj m_collision;
};
