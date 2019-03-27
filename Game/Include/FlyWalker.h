#pragma once
#include "Walker.h"
class FlyWalker : public Walker {
public:
	FlyWalker();
	~FlyWalker();

	/// <summary>
	/// 飛行させます。
	/// </summary>
	/// <remarks>isFlyがtrueなら飛び立つとき、falseなら飛んでる最中の処理。
	/// 飛行中はUpdate関数でisFlyをfalseにして呼び続けてください。
	/// 速さは秒速ですが空気抵抗があるので大き目の数字にしないと速くなりません。</remarks>
	/// <param name="isFly">飛行の最初の処理ならtrue</param>
	/// <param name="looking">視線ベクトル(正規化する)</param>
	/// <param name="move">パッドの左スティック入力</param>
	/// <param name="power">飛行の速さ</param>
	void fly(bool isFly, const CVector3& looking, const CVector2& move, float power = 0.0f);

	/// <summary>
	/// 飛行を停止します
	/// </summary>
	void flyStop();

	/// <summary>
	/// 飛行不可時間にします。
	/// </summary>
	void rest();
	/// <summary>
	/// 飛行可能にします。
	/// </summary>
	void restStop();

	CQuaternion getRotation() const;

	/// <summary>
	/// 飛行可能残り時間を取得
	/// </summary>
	/// <returns></returns>
	float getFlyTimer() const{
		return flyTimer;
	}
	/// <summary>
	/// 最大飛行可能時間を取得
	/// </summary>
	/// <returns></returns>
	float getFlyTimerMax() const{
		return c_flyTimer;
	}

	/// <summary>
	/// 飛行終了後の回復開始までのクールタイムを取得
	/// </summary>
	/// <returns></returns>
	float getCoolTimer() const {
		return coolTimer;
	}

	bool isFlying()const {
		return flying;
	}

	/// <summary>
	/// 壁に衝突したときtrueを返す
	/// </summary>
	/// <returns></returns>
	bool isHitWall() const {
		return hitWall;
	}

	/// <summary>
	/// 飛行不可時間中ならtrueを返す
	/// </summary>
	/// <returns></returns>
	bool isFlyRest() const {
		return m_isRest;
	}

	/// <summary>
	/// 飛行の速さを取得
	/// </summary>
	/// <returns></returns>
	float GetFlyPower()const {
		return flyPower;
	}

	//通信受信で使う
	void SetIsFly(bool f) { flying = f; }
	void SetFlyPower(float p) { flyPower = p; }

	//飛行可能時間をセットする
	void SetFlyTimer(float timer) { flyTimer = timer; }
	void SetCoolTimer(float timer) { coolTimer = timer; }

	void SetIsLocalUser(bool is) { m_isLocalUser = is; }

	/// <summary>
	/// 対象のUpdate関数で呼ぶ
	/// </summary>
	void Update();

	static constexpr float c_flyTimer = 5.0f;//最大飛行可能時間
private:
	bool m_isLocalUser = true;

	float flyTimer = c_flyTimer; //飛行可能な残り時間
	bool m_isRest = 0.0f; //飛行不可ならtrue

	static constexpr float c_coolTimer = 1.0f;
	float coolTimer = 0.0f; //断続飛び防止のクールタイム

	GameObj::Suicider::CEffekseer* restEffect = nullptr;//飛行不可時間中のエフェクト
	GameObj::Suicider::CEffekseer* recoverEffect = nullptr;//飛行不可回復のエフェクト
	static constexpr float c_restEfkTimer = 1.0f;
	float m_restEfkTimer = c_restEfkTimer;//エフェクトを一定時間置きに再生するためのタイマー
	static constexpr float c_recoverEfkTime = 1.0f;

	float upDown = 0.0f;//上下方向の移動

	bool hitWall = false;//壁に衝突したらtrueになる

	CQuaternion springRot;//滑らかなバネ回転のための四元数

	float flyPower = 0.0f;
	bool flying = false;

	SuicideObj::CSE* se = nullptr;//飛行中の風切り音
};

