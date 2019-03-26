#pragma once

#include "FlyWalker.h"
#include "ActionSender.h"
#include "Team.h"
#include "MiniHPbar.h"
#include "../Ginger/Ginger.h"
#include "IDamagable.h"
#include "SE_Util.h"

class NetPlayerCaster;

class Weapon;
class Bullet;
class CPlayerCamera;
class Wing;

class CPlayer : public IGameObject , public IDamagable{
public:
	CPlayer(int playerNum, Team* team, const CVector3& positon);
	virtual ~CPlayer();

	bool Start() override;
	virtual void Update() override;
	
	void sendAction(const ActionSender& action);

	bool damage(const CVector3& dir, unsigned int damage,
				const Team* team = nullptr, const CPlayer* = nullptr) override;

	const btCollisionObject* getCollisionObj() {
		return &m_collision.GetCollisionObject();
	}

	bool isFlying()const {
		return mover.isFlying();
	}
	float getFlyPower()const {
		return mover.GetFlyPower();
	}
	bool isRest()const {
		return mover.isRest();
	}

	const CVector3& getPosition() const {
		return mover.GetPosition();
	}

	CVector3 getVelocity() const{
		return mover.getVelocity();
	}

	CQuaternion getRotation() const{
		return mover.getRotation();
	}

	Team* team;

	const int playerNum;

	virtual void SetGodPower(GodPowerType type) {
		//�G�t�F�N�g
		SuicideObj::CEffekseer* effe = new SuicideObj::CEffekseer(L"Resource/effect/aura.efk", 1.0f, getPosition());
		effe->SetScale({ 50.0f,50.0f ,50.0f });
		//SE
		SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_getPower.wav");
		se->SetPos(getPosition());//���̈ʒu
		se->SetDistance(500.0f);//������������͈�
		se->Play(true);
	}

	//�ʐM���M�Ŏg�p
	const ActionSender& GetActionSender()const {
		return action;
	}
	float getFlyTimer()const {
		return mover.getFlyTimer();
	}
	float getCoolTimer()const {
		return mover.getCoolTimer();
	}
	bool GetIsDead()const {
		return m_hp == 0 ? true : false;
	}
	Weapon** GetWeapons() {
		return weapon;
	}
	int GetWeaponNum()const {
		return WEAPON_NUM;
	}
	unsigned char GetActiveWeapon()const {
		return activeWeapon;
	}
	//�ʐM��M�Ŏg�p
	void SetPosition(const CVector3& pos) {
		mover.SetPosition(pos);
	}
	void SetVelocity(const CVector3& vel) {
		mover.SetVelocity(vel);
	}
	void SetIsFly(bool f) { mover.SetIsFly(f); }
	void SetFlyTimer(float p) { mover.SetFlyTimer(p); }
	void SetCoolTimer(float p) { mover.SetCoolTimer(p); }
	void fly() {
		mover.fly(true, action.getLookVec(),{0,0}, flyPower);
	}
	void flyStop() {
		mover.flyStop();
	}
	void rest() {
		mover.rest();
	}
	void restStop() {
		mover.restStop();
	}
	void changeWeapon(unsigned char useWeapon);
	bool GetIsInit()const { return m_Init; }

	//���b�N�I��
	void SetLockOn(bool isply, int num) { m_lockIsPly = isply; m_lockonNum = num; }
	bool GetLockOnIsPly() const { return m_lockIsPly; }
	int  GetLockOnNum() const{ return m_lockonNum; }

	//���f�����擾
	GameObj::CSkinModelRender& GetModel() { return m_model; }

	//���S����
	void Death();
	//�h������
	void Revive();

	static constexpr float animInterpolateSec = 0.2f;        //�A�j���[�V������Ԏ���
	enum {//����̗�
		HUND_GUN,
		RIFLE,
		LAZER,
		WEAPON_NUM
	};
private:
	void Move();
	void Shot();
	void Reload();
	void changeWeapon(bool left, bool Right);

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

protected:
	static constexpr unsigned short maxHp = 1000;
	unsigned short m_hp = maxHp;
	MiniHPbar miniHpbar;
	FlyWalker mover;    //�����̊Ǘ�
	unsigned char activeWeapon = -1;
	Weapon* weapon[WEAPON_NUM]; //����
	bool m_lockIsPly = true; int m_lockonNum = -1; //���b�N�I���Ώ�
private:
	Wing* wing = nullptr; //��

	static constexpr float constDeathCool = 10;
	float deathCool = 0;

	CQuaternion m_rot;
	float radian = 0.0f; //��]��

	static constexpr float flyPower = 60000.0f; //��s��
	static constexpr float jumpPower = 600.0f; //�W�����v��
	static constexpr float moveSpeed = 40.0f; //�ړ����x
	static constexpr float dashMul = 2.0f; //�_�b�V���{��

	ActionSender action;     //�v���C���[�̑��삪�����Ă���

	SuicideObj::CCollisionObj m_collision; //�R���W����

//////�ʐM
	bool m_Init = false;
	//�ʐM�L���X�^�[
	NetPlayerCaster* m_netCaster = nullptr;
	//���M�W�����vSE
	bool m_isSendJumpSE = false;
public:
	void SetNetCaster(NetPlayerCaster* netcaster) { m_netCaster = netcaster; }
	NetPlayerCaster* GetNetCaster() const { return m_netCaster; }

	void OffIsSendJumpSE() { m_isSendJumpSE = false; }
	bool GetIsSendJumpSE()const { return m_isSendJumpSE; }
	void PlayJumpSE() {
		playSE(L"Resource/sound/SE_jump.wav");
	}
};
