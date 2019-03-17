#pragma once

#include "FlyWalker.h"
#include "ActionSender.h"
#include "Team.h"
#include "MiniHPbar.h"

class NetPlayerCaster;

class Weapon;
class Bullet;
class CPlayerCamera;
class Wing;

class CPlayer : public IGameObject {
public:
	CPlayer(int playerNum, Team* team, const CVector3& positon);
	virtual ~CPlayer();

	bool Start() override;
	virtual void Update() override;
	
	void sendAction(const ActionSender& action);

	bool BatHit(Bullet* bullet);

	void Hit(const CVector3& dir, unsigned int damage);

	const btCollisionObject* getCollisionObj() {
		return &m_collision.GetCollisionObject();
	}

	bool isFlying()const {
		return mover.isFlying();
	}
	float getFlyPower()const {
		return mover.GetFlyPower();
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


	//通信送信で使用
	const ActionSender& GetActionSender()const {
		return action;
	}
	float getFlyTimer()const {
		return mover.getFlyTimer();
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
	//通信受信で使用
	void SetPosition(const CVector3& pos) {
		mover.SetPosition(pos);
	}
	void SetVelocity(const CVector3& vel) {
		mover.SetVelocity(vel);
	}
	void SetIsFly(bool f) { mover.SetIsFly(f); }
	void SetFlyTimer(float p) { mover.SetFlyTimer(p); }
	void fly() {
		mover.fly(true, action.getLookVec(),{0,0}, flyPower);
	}
	void flyStop() {
		mover.flyStop();
	}
	void changeWeapon(unsigned char useWeapon);
	bool GetIsInit()const { return m_Init; }

	//ロックオン
	void SetLockOn(bool isply, int num) { m_lockIsPly = isply; m_lockonNum = num; }
	bool GetLockOnIsPly() const { return m_lockIsPly; }
	int  GetLockOnNum() const{ return m_lockonNum; }

	//死亡処理
	void Death();
	//蘇生処理
	void Revive();

	static constexpr float animInterpolateSec = 0.2f;        //アニメーション補間時間
	enum {//武器の列挙
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

protected:
	static constexpr unsigned short maxHp = 1000;
	unsigned short m_hp = maxHp;
	MiniHPbar miniHpbar;
	FlyWalker mover;    //動きの管理
	unsigned char activeWeapon = -1;
	Weapon* weapon[WEAPON_NUM]; //武器
	bool m_lockIsPly = true; int m_lockonNum = -1; //ロックオン対象
private:
	Wing* wing = nullptr; //翼

	static constexpr float constDeathCool = 10;
	float deathCool = 0;

	CQuaternion m_rot;
	float radian = 0.0f; //回転量

	static constexpr float flyPower = 60000.0f; //飛行力
	static constexpr float jumpPower = 600.0f; //ジャンプ力
	static constexpr float moveSpeed = 40.0f; //移動速度
	static constexpr float dashMul = 2.0f; //ダッシュ倍率

	ActionSender action;     //プレイヤーの操作が入っている

	SuicideObj::CCollisionObj m_collision; //コリジョン

	//通信
	bool m_Init = false;
	//通信キャスター
	NetPlayerCaster* m_netCaster = nullptr;
public:
	void SetNetCaster(NetPlayerCaster* netcaster) { m_netCaster = netcaster; }
	NetPlayerCaster* GetNetCaster() { return m_netCaster; }
};
