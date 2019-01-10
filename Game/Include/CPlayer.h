#pragma once

#include "DemolisherWeapon/physics/character/CCharacterController.h"
#include "ActionSender.h"
#include "Team.h"

class CPlayerCamera;

class CPlayer : public IGameObject {
public:
	CPlayer(int playerNum, Team* team, const CVector3& positon);
	virtual ~CPlayer();

	bool Start() override;
	virtual void Update() override;

	CVector3 getPosition();

	void sendAction(const ActionSender& action);

	bool BatHit(CPlayer* player,const CVector3& dir);

	void Hit(const CVector3& dir);

	const btCollisionObject* getCollisionObj() {
		return &m_collision.GetCollisionObject();
	}

	Team* team;

	const int playerNum;


	//�ʐM���M�Ŏg�p
	const ActionSender& GetActionSender()const {
		return action;
	}
	bool GetIsDead()const {
		return m_hp == 0 ? true : false;
	}
	//�ʐM��M�Ŏg�p
	void SetPosition(const CVector3& pos) {
		m_pos = pos;
		charaCon.SetPosition(m_pos);
	}

	//���S����
	void Death();
	//�h������
	void Revive();

private:
	void GravityAndJump();
	void Move();
	void Turn();
	void Shot();
	void Reload();

	GameObj::CSkinModelRender m_model;
	enum {
		anim_run,
		anim_walk,
		anim_idle,
		anim_shot,
		anim_reload,
		anim_num,
	};
	AnimationClip m_animationClips[anim_num];
	static constexpr float animInterpolateSec = 0.2f;        //�A�j���[�V������Ԏ���

protected:
	static constexpr unsigned short constHp = 10;
	unsigned short m_hp = 10;
private:
	static constexpr float constDeathCool = 10;
	float deathCool = 0;

	CVector3 m_pos;
	CQuaternion m_rot;
	float radian = 0.0f; //��]��

	//�e�n
	static constexpr float constShotCool = 0.1f;
	float shotCool = constShotCool;
	static constexpr unsigned int constBulletCount = 50;
	unsigned int bulletCount = constBulletCount;
	bool onReload = false;

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
