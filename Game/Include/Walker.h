#pragma once
#include "DemolisherWeapon/physics/character/CCharacterController.h"

class Walker : public CCharacterController{
public:
	Walker();
	~Walker();

	void move(CVector2 move);
	void jump(float jumpPower);

	/// <summary>
	/// �ǂɒ���t���Ă���ꍇ�ɕǃL�b�N������
	/// </summary>
	/// <remarks>�ǂɒ���t���Ă��Ȃ��ꍇ���s���Ă����������܂���B
	/// �ǂ̔��Ε����ɔ�т܂���move�̒l�ŕ�����������ł��܂��B</remarks>
	/// <param name="jumpPower">�W�����v��</param>
	/// <param name="move">�W�����v�̕���</param>
	void walljump(float jumpPower, CVector2 move);
	void Update();

	void turn(float x, float z);

	/// <summary>
	/// ���x�����Z���܂�
	/// </summary>
	/// <param name="velocity">���Z����x�N�g��</param>
	void addVelocity(const CVector3& velocity);

	CVector3 getVelocity()  const{
		return velocity;
	}

	/// <summary>
	/// ���x��ݒ肵�܂�
	/// </summary>
	/// <param name="v">�ݒ肷��x�N�g��</param>
	void SetVelocity(const CVector3& v) {
		velocity = v;
	}

	CQuaternion getRotation() const{
		return springRot;
	}

protected:
	//�o�l��]
	virtual void springRotation();

	CVector3 velocity = { 0.0f, 0.0f, 0.0f };	 //���x

	CQuaternion m_rot;
	CQuaternion springRot;//�o�l��]�̂��߂̎l����

	bool beforeGround = true;

	static constexpr float friction = 10.0f; //���C
	static constexpr float gravity = 980.0f; //�d�͉����x

	SuicideObj::CSE* footSe = nullptr;
	static constexpr float c_footTime = 150.0f;//������炷�^�C�~���O
	float footTime = 0.0f;
};

