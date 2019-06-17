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
	
	/// <summary>
	/// �v���C���[�ɑ΂���l�X�ȑ����`����B
	/// </summary>
	/// <param name="action">���삪�i�[���ꂽActionSender</param>
	void sendAction(const ActionSender& action);

	/// <summary>
	/// �_���[�W��^���鏈���B
	/// </summary>
	/// <remarks>
	/// �_���[�W��HP��������A�����Ƃ��Ĕ�_���҂Ƀx�N�g����^���邱�Ƃ����܂��B
	/// �߂�l��false���Ԃ����ꍇ�A�U���͓������Ă��܂���B(���łɃv���C���[�����S���Ă���ꍇ�Ȃ�)
	/// </remarks>
	/// <param name="dir">�����x�N�g��</param>
	/// <param name="damage">�_���[�W</param>
	/// <param name="team">�U���҂̃`�[��</param>
	/// <param name="">�U�������v���C���[</param>
	/// <returns>�U�������������ꍇ��true</returns>
	bool damage(const CVector3& dir, unsigned int damage,
				const Team* team = nullptr, const CPlayer* = nullptr) override;

	const btCollisionObject* getCollisionObj() {
		return &m_collision.GetCollisionObject();
	}

	bool isFlying()const {
		return mover.isFlying();
	}

	/// <summary>
	/// ��s���x���擾����B
	/// </summary>
	/// <returns>��s���x(�b��)</returns>
	float getFlyPower()const {
		return mover.GetFlyPower();
	}

	/// <summary>
	/// ��s�s���Ԓ��Ȃ�true��Ԃ�
	/// </summary>
	/// <returns></returns>
	bool isFlyRest()const {
		return mover.isFlyRest();
	}

	const CVector3& getPosition() const {
		return mover.GetPosition();
	}
	/// <summary>
	/// �d�͂��擾
	/// </summary>
	CVector3 getVelocity() const{
		return mover.getVelocity();
	}

	/// <summary>
	/// ��]���擾
	/// </summary>
	/// <returns></returns>
	CQuaternion getRotation() const{
		return mover.getRotation();
	}

	/// <summary>
	/// �`�[�����擾
	/// </summary>
	/// <returns></returns>
	Team* getTeam() const{
		return team;
	}

	/// <summary>
	/// �`�[����ݒ�
	/// </summary>
	/// <param name="team"></param>
	void setTeam(Team* team) {
		this->team = team;
	}

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
	/// <summary>
	/// ��s�\���Ԃ��擾
	/// </summary>
	/// <returns></returns>
	float getFlyTimer()const {
		return mover.getFlyTimer();
	}
	/// <summary>
	/// ��s���~��ɔ�s�\���Ԃ��񕜂��n�߂�܂ł̃N�[���^�C�����擾
	/// </summary>
	/// <returns></returns>
	float getCoolTimer()const {
		return mover.getCoolTimer();
	}

	/// <summary>
	/// ����ł���Ȃ�true��Ԃ�
	/// </summary>
	/// <returns></returns>
	bool GetIsDead()const {
		return m_hp == 0 ? true : false;
	}

	/// <summary>
	/// ����z����擾
	/// </summary>
	/// <returns></returns>
	Weapon** GetWeapons() {
		return weapon;
	}
	/// <summary>
	/// ����z��̗v�f�����擾
	/// </summary>
	/// <returns></returns>
	int GetWeaponNum()const {
		return WEAPON_NUM;
	}

	/// <summary>
	/// ��Ɏ����Ă��镐��̗v�f�ԍ����擾
	/// </summary>
	/// <returns></returns>
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
	/// <summary>
	/// �����ύX���܂��B
	/// </summary>
	/// <param name="useWeapon">�ύX���镐��̗v�f�ԍ�</param>
	void changeWeapon(unsigned char useWeapon);

	bool GetIsInit()const { return m_Init; }

	//���b�N�I��
	void SetLockOn(bool isply, int num) { m_lockIsPly = isply; m_lockonNum = num; }
	bool GetLockOnIsPly() const { return m_lockIsPly; }
	int  GetLockOnNum() const{ return m_lockonNum; }

	//�C���r�W�u���t���O
	void SetInvisible(bool flag) { m_invisible = flag; }
	bool GetInvisible() const{ return m_invisible; }

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
	/// <summary>
	/// �����ύX����B
	/// </summary>
	/// <remarks>
	/// left�Ȃ�}�C�i�X�Aright�Ȃ�v���X�����ɕ���z����ړ����܂��B
	/// ���������ꂽ�ꍇ�A�Ȃɂ����܂���B</remarks>
	/// <param name="left">true�Ȃ�}�C�i�X�����Ɉړ�</param>
	/// <param name="Right">true�Ȃ�v���X�����Ɉړ�</param>
	void changeWeapon(bool left, bool right);

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
	SkeletonIK::IKSetting* m_bodyIKSetting = nullptr;//����IK�ݒ�
	CVector3 m_bodyIKtarget;

	Team* team;

protected:
	static constexpr unsigned short maxHp = 1000;
	unsigned short m_hp = maxHp;
	MiniHPbar miniHpbar;//����ɕ\�������HP�o�[
	FlyWalker mover;    //�����̊Ǘ�
	unsigned char activeWeapon = -1;//�A�N�e�B�u�ȕ���̗v�f�ԍ�
	Weapon* weapon[WEAPON_NUM]; //����
	bool m_lockIsPly = true; int m_lockonNum = -1; //���b�N�I���Ώ�
	bool m_invisible = false;//�C���r�W�u���t���O
private:
	Wing* wing = nullptr; //��

	static constexpr float constDeathCool = 10;//�h���܂ł̎���ł��鎞��
	float deathCool = 0;//�����݂̑h������܂ł̎���

	CQuaternion m_rot;//��]

	static constexpr float flyPower = 70000.0f; //��s��
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
	void PlayJumpSE();
};
