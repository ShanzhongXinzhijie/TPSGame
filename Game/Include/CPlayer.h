#pragma once

#include "DemolisherWeapon/physics/character/CCharacterController.h"
#include "ActionSender.h"

class CPlayerCamera;

class CPlayer : public IGameObject {
public:
	CPlayer(int playerNum, CVector4 color, const CVector3& positon);
	virtual ~CPlayer();

	bool Start() override;
	virtual void Update() override;

	CVector3 getPosition();

	void sendAction(const ActionSender& action);

	bool BatHit(CPlayer* player, CVector3 dir);

	const btCollisionObject* getCollisionObj() {
		return &m_collision.GetCollisionObject();
	}

	const CVector4 color;

	const int playerNum;


	//�ʐM���M�Ŏg�p
	const ActionSender& GetActionSender()const {
		return action;
	}
	bool GetIsDead()const {
		return m_hp == 0 ? true : false;
	}

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
		anim_shot,
		anim_num,
	};
	AnimationClip m_animationClips[anim_num];
	static constexpr float animInterpolateSec = 0.2f;        //�A�j���[�V������Ԏ���

	static constexpr unsigned short constHp = 10;
	unsigned short m_hp = constHp;
	static constexpr float constDeathCool = 10;
	float deathCool = 0;

	CVector3 m_pos;
	CQuaternion m_rot;
	float radian = 0.0f; //��]��

	//�e�̃N�[���^�C��
	static constexpr float constShotCool = 0.1f;
	float shotCool = constShotCool;

	//�����n
	static constexpr float moveSpeed = 80.0f;              //�ړ����x
	static constexpr float dashMul = 2.0f;	             //�_�b�V���{��
	static constexpr float friction = 10.0f;	             //���C
	static constexpr float gravity = 980.0f;	             //�d�͉����x
	static constexpr float jumpPower = 600.0f;             //�W�����v��

	CVector3 velocity = { 0.0f, 0.0f, 0.0f };	 //���x
	CCharacterController charaCon;
	ActionSender action;     //�v���C���[�̑��삪�����Ă���

	SuicideObj::CCollisionObj m_collision; //�R���W����
};
