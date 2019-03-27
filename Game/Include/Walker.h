#pragma once
#include "DemolisherWeapon/physics/character/CCharacterController.h"

class Walker : public CCharacterController{
public:
	Walker();
	~Walker();

	void move(CVector2 move);
	void jump(float jumpPower);

	/// <summary>
	/// 壁に張り付いている場合に壁キックをする
	/// </summary>
	/// <remarks>壁に張り付いていない場合実行しても何もおきません。
	/// 壁の反対方向に飛びますがmoveの値で方向を微調整できます。</remarks>
	/// <param name="jumpPower">ジャンプ力</param>
	/// <param name="move">ジャンプの方向</param>
	void walljump(float jumpPower, CVector2 move);
	void Update();

	void turn(float x, float z);

	/// <summary>
	/// 速度を加算します
	/// </summary>
	/// <param name="velocity">加算するベクトル</param>
	void addVelocity(const CVector3& velocity);

	CVector3 getVelocity()  const{
		return velocity;
	}

	/// <summary>
	/// 速度を設定します
	/// </summary>
	/// <param name="v">設定するベクトル</param>
	void SetVelocity(const CVector3& v) {
		velocity = v;
	}

	CQuaternion getRotation() const{
		return springRot;
	}

protected:
	//バネ回転
	virtual void springRotation();

	CVector3 velocity = { 0.0f, 0.0f, 0.0f };	 //速度

	CQuaternion m_rot;
	CQuaternion springRot;//バネ回転のための四元数

	bool beforeGround = true;

	static constexpr float friction = 10.0f; //摩擦
	static constexpr float gravity = 980.0f; //重力加速度

	SuicideObj::CSE* footSe = nullptr;
	static constexpr float c_footTime = 150.0f;//足音を鳴らすタイミング
	float footTime = 0.0f;
};

