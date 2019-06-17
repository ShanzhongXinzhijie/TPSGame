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
	/// プレイヤーに対する様々な操作を伝える。
	/// </summary>
	/// <param name="action">操作が格納されたActionSender</param>
	void sendAction(const ActionSender& action);

	/// <summary>
	/// ダメージを与える処理。
	/// </summary>
	/// <remarks>
	/// ダメージ分HPから引き、反動として被ダメ者にベクトルを与えることもします。
	/// 戻り値にfalseが返った場合、攻撃は当たっていません。(すでにプレイヤーが死亡している場合など)
	/// </remarks>
	/// <param name="dir">反動ベクトル</param>
	/// <param name="damage">ダメージ</param>
	/// <param name="team">攻撃者のチーム</param>
	/// <param name="">攻撃したプレイヤー</param>
	/// <returns>攻撃が成功した場合はtrue</returns>
	bool damage(const CVector3& dir, unsigned int damage,
				const Team* team = nullptr, const CPlayer* = nullptr) override;

	const btCollisionObject* getCollisionObj() {
		return &m_collision.GetCollisionObject();
	}

	bool isFlying()const {
		return mover.isFlying();
	}

	/// <summary>
	/// 飛行速度を取得する。
	/// </summary>
	/// <returns>飛行速度(秒速)</returns>
	float getFlyPower()const {
		return mover.GetFlyPower();
	}

	/// <summary>
	/// 飛行不可時間中ならtrueを返す
	/// </summary>
	/// <returns></returns>
	bool isFlyRest()const {
		return mover.isFlyRest();
	}

	const CVector3& getPosition() const {
		return mover.GetPosition();
	}
	/// <summary>
	/// 重力を取得
	/// </summary>
	CVector3 getVelocity() const{
		return mover.getVelocity();
	}

	/// <summary>
	/// 回転を取得
	/// </summary>
	/// <returns></returns>
	CQuaternion getRotation() const{
		return mover.getRotation();
	}

	/// <summary>
	/// チームを取得
	/// </summary>
	/// <returns></returns>
	Team* getTeam() const{
		return team;
	}

	/// <summary>
	/// チームを設定
	/// </summary>
	/// <param name="team"></param>
	void setTeam(Team* team) {
		this->team = team;
	}

	const int playerNum;

	virtual void SetGodPower(GodPowerType type) {
		//エフェクト
		SuicideObj::CEffekseer* effe = new SuicideObj::CEffekseer(L"Resource/effect/aura.efk", 1.0f, getPosition());
		effe->SetScale({ 50.0f,50.0f ,50.0f });
		//SE
		SuicideObj::CSE* se = NewGO<SuicideObj::CSE>(L"Resource/sound/SE_getPower.wav");
		se->SetPos(getPosition());//音の位置
		se->SetDistance(500.0f);//音が聞こえる範囲
		se->Play(true);
	}

	//通信送信で使用
	const ActionSender& GetActionSender()const {
		return action;
	}
	/// <summary>
	/// 飛行可能時間を取得
	/// </summary>
	/// <returns></returns>
	float getFlyTimer()const {
		return mover.getFlyTimer();
	}
	/// <summary>
	/// 飛行中止後に飛行可能時間が回復し始めるまでのクールタイムを取得
	/// </summary>
	/// <returns></returns>
	float getCoolTimer()const {
		return mover.getCoolTimer();
	}

	/// <summary>
	/// 死んでいるならtrueを返す
	/// </summary>
	/// <returns></returns>
	bool GetIsDead()const {
		return m_hp == 0 ? true : false;
	}

	/// <summary>
	/// 武器配列を取得
	/// </summary>
	/// <returns></returns>
	Weapon** GetWeapons() {
		return weapon;
	}
	/// <summary>
	/// 武器配列の要素数を取得
	/// </summary>
	/// <returns></returns>
	int GetWeaponNum()const {
		return WEAPON_NUM;
	}

	/// <summary>
	/// 手に持っている武器の要素番号を取得
	/// </summary>
	/// <returns></returns>
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
	/// 武器を変更します。
	/// </summary>
	/// <param name="useWeapon">変更する武器の要素番号</param>
	void changeWeapon(unsigned char useWeapon);

	bool GetIsInit()const { return m_Init; }

	//ロックオン
	void SetLockOn(bool isply, int num) { m_lockIsPly = isply; m_lockonNum = num; }
	bool GetLockOnIsPly() const { return m_lockIsPly; }
	int  GetLockOnNum() const{ return m_lockonNum; }

	//インビジブルフラグ
	void SetInvisible(bool flag) { m_invisible = flag; }
	bool GetInvisible() const{ return m_invisible; }

	//モデルを取得
	GameObj::CSkinModelRender& GetModel() { return m_model; }

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
	/// <summary>
	/// 武器を変更する。
	/// </summary>
	/// <remarks>
	/// leftならマイナス、rightならプラス方向に武器配列を移動します。
	/// 両方押された場合、なにもしません。</remarks>
	/// <param name="left">trueならマイナス方向に移動</param>
	/// <param name="Right">trueならプラス方向に移動</param>
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
	SkeletonIK::IKSetting* m_bodyIKSetting = nullptr;//胴体IK設定
	CVector3 m_bodyIKtarget;

	Team* team;

protected:
	static constexpr unsigned short maxHp = 1000;
	unsigned short m_hp = maxHp;
	MiniHPbar miniHpbar;//頭上に表示されるHPバー
	FlyWalker mover;    //動きの管理
	unsigned char activeWeapon = -1;//アクティブな武器の要素番号
	Weapon* weapon[WEAPON_NUM]; //武器
	bool m_lockIsPly = true; int m_lockonNum = -1; //ロックオン対象
	bool m_invisible = false;//インビジブルフラグ
private:
	Wing* wing = nullptr; //翼

	static constexpr float constDeathCool = 10;//蘇生までの死んでいる時間
	float deathCool = 0;//今現在の蘇生するまでの時間

	CQuaternion m_rot;//回転

	static constexpr float flyPower = 70000.0f; //飛行力
	static constexpr float jumpPower = 600.0f; //ジャンプ力
	static constexpr float moveSpeed = 40.0f; //移動速度
	static constexpr float dashMul = 2.0f; //ダッシュ倍率

	ActionSender action;     //プレイヤーの操作が入っている

	SuicideObj::CCollisionObj m_collision; //コリジョン

//////通信
	bool m_Init = false;
	//通信キャスター
	NetPlayerCaster* m_netCaster = nullptr;
	//送信ジャンプSE
	bool m_isSendJumpSE = false;
public:
	void SetNetCaster(NetPlayerCaster* netcaster) { m_netCaster = netcaster; }
	NetPlayerCaster* GetNetCaster() const { return m_netCaster; }

	void OffIsSendJumpSE() { m_isSendJumpSE = false; }
	bool GetIsSendJumpSE()const { return m_isSendJumpSE; }
	void PlayJumpSE();
};
