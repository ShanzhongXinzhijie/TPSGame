#pragma once

#include "FlyWalker.h"
#include "ActionSender.h"
#include "Team.h"

class CPlayerCamera;

class CPlayer : public IGameObject {
public:
	CPlayer(int playerNum, Team* team, const CVector3& positon);
	virtual ~CPlayer();

	bool Start() override;
	virtual void Update() override;

	CVector3 getPosition() const;

	void sendAction(const ActionSender& action);

	bool BatHit(CPlayer* player,const CVector3& dir);

	void Hit(const CVector3& dir);

	const btCollisionObject* getCollisionObj() {
		return &m_collision.GetCollisionObject();
	}

	bool isFlying() {
		return mover.isFlying();
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
		mover.SetPosition(pos);
	}

	//���S����
	void Death();
	//�h������
	void Revive();

private:
	void Move();
	void Shot();
	void Reload();
	void playSE(const wchar_t* path);

	GameObj::CSkinModelRender m_model;
	enum {
		anim_run,
		anim_walk,
		anim_fly,
		anim_idle,
		anim_shot,
		anim_reload,
		anim_jump,
		anim_fall,
		anim_num,
	};
	AnimationClip m_animationClips[anim_num];
	static constexpr float animInterpolateSec = 0.2f;        //�A�j���[�V������Ԏ���

protected:
	static constexpr unsigned short constHp = 10;
	unsigned short m_hp = 10;
	FlyWalker mover;    //�����̊Ǘ�
	static constexpr unsigned int constBulletCount = 50;
	unsigned int bulletCount = constBulletCount;

private:
	static constexpr float constDeathCool = 10;
	float deathCool = 0;

	CQuaternion m_rot;
	float radian = 0.0f; //��]��

	static constexpr float constShotCool = 0.1f;
	float shotCool = constShotCool;
	bool onReload = false;

	static constexpr float flyPower = 180000.0f; //��s��
	static constexpr float jumpPower = 600.0f; //�W�����v��
	static constexpr float moveSpeed = 80.0f; //�ړ����x
	static constexpr float dashMul = 2.0f; //�_�b�V���{��

	ActionSender action;     //�v���C���[�̑��삪�����Ă���

	SuicideObj::CCollisionObj m_collision; //�R���W����
};
