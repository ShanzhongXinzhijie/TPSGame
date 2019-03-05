#pragma once

#include "FlyWalker.h"
#include "ActionSender.h"
#include "Team.h"

class Weapon;
#include "../NetWork/NetPlayerCaster.h"

class CPlayerCamera;
class Wing;

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

	CVector3 getVelocity() {
		return mover.getVelocity();
	}

	Team* team;

	const int playerNum;


	//通信送信で使用
	const ActionSender& GetActionSender()const {
		return action;
	}
	bool GetIsDead()const {
		return m_hp == 0 ? true : false;
	}
	//通信受信で使用
	void SetPosition(const CVector3& pos) {
		mover.SetPosition(pos);
	}

	//死亡処理
	void Death();
	//蘇生処理
	void Revive();

	static constexpr float animInterpolateSec = 0.2f;        //アニメーション補間時間
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

protected:
	static constexpr unsigned short constHp = 10;
	unsigned short m_hp = 10;
	FlyWalker mover;    //動きの管理
	Weapon* weapon[1]; //武器
private:
	Wing* wing = nullptr; //翼

	static constexpr float constDeathCool = 10;
	float deathCool = 0;

	CQuaternion m_rot;
	float radian = 0.0f; //回転量

	static constexpr float flyPower = 180000.0f; //飛行力
	static constexpr float jumpPower = 600.0f; //ジャンプ力
	static constexpr float moveSpeed = 80.0f; //移動速度
	static constexpr float dashMul = 2.0f; //ダッシュ倍率

	ActionSender action;     //プレイヤーの操作が入っている

	SuicideObj::CCollisionObj m_collision; //コリジョン

	//通信キャスター
	NetPlayerCaster* m_netCaster = nullptr;
public:
	void SetNetCaster(NetPlayerCaster* netcaster) { m_netCaster = netcaster; }
	NetPlayerCaster* GetNetCaster() { return m_netCaster; }
};
