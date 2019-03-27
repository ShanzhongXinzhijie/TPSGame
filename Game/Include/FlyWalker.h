#pragma once
#include "Walker.h"
class FlyWalker : public Walker {
public:
	FlyWalker();
	~FlyWalker();

	/// <summary>
	/// ��s�����܂��B
	/// </summary>
	/// <remarks>isFly��true�Ȃ��ї��Ƃ��Afalse�Ȃ���ł�Œ��̏����B
	/// ��s����Update�֐���isFly��false�ɂ��ČĂё����Ă��������B
	/// �����͕b���ł�����C��R������̂ő傫�ڂ̐����ɂ��Ȃ��Ƒ����Ȃ�܂���B</remarks>
	/// <param name="isFly">��s�̍ŏ��̏����Ȃ�true</param>
	/// <param name="looking">�����x�N�g��(���K������)</param>
	/// <param name="move">�p�b�h�̍��X�e�B�b�N����</param>
	/// <param name="power">��s�̑���</param>
	void fly(bool isFly, const CVector3& looking, const CVector2& move, float power = 0.0f);

	/// <summary>
	/// ��s���~���܂�
	/// </summary>
	void flyStop();

	/// <summary>
	/// ��s�s���Ԃɂ��܂��B
	/// </summary>
	void rest();
	/// <summary>
	/// ��s�\�ɂ��܂��B
	/// </summary>
	void restStop();

	CQuaternion getRotation() const;

	/// <summary>
	/// ��s�\�c�莞�Ԃ��擾
	/// </summary>
	/// <returns></returns>
	float getFlyTimer() const{
		return flyTimer;
	}
	/// <summary>
	/// �ő��s�\���Ԃ��擾
	/// </summary>
	/// <returns></returns>
	float getFlyTimerMax() const{
		return c_flyTimer;
	}

	/// <summary>
	/// ��s�I����̉񕜊J�n�܂ł̃N�[���^�C�����擾
	/// </summary>
	/// <returns></returns>
	float getCoolTimer() const {
		return coolTimer;
	}

	bool isFlying()const {
		return flying;
	}

	/// <summary>
	/// �ǂɏՓ˂����Ƃ�true��Ԃ�
	/// </summary>
	/// <returns></returns>
	bool isHitWall() const {
		return hitWall;
	}

	/// <summary>
	/// ��s�s���Ԓ��Ȃ�true��Ԃ�
	/// </summary>
	/// <returns></returns>
	bool isFlyRest() const {
		return m_isRest;
	}

	/// <summary>
	/// ��s�̑������擾
	/// </summary>
	/// <returns></returns>
	float GetFlyPower()const {
		return flyPower;
	}

	//�ʐM��M�Ŏg��
	void SetIsFly(bool f) { flying = f; }
	void SetFlyPower(float p) { flyPower = p; }

	//��s�\���Ԃ��Z�b�g����
	void SetFlyTimer(float timer) { flyTimer = timer; }
	void SetCoolTimer(float timer) { coolTimer = timer; }

	void SetIsLocalUser(bool is) { m_isLocalUser = is; }

	/// <summary>
	/// �Ώۂ�Update�֐��ŌĂ�
	/// </summary>
	void Update();

	static constexpr float c_flyTimer = 5.0f;//�ő��s�\����
private:
	bool m_isLocalUser = true;

	float flyTimer = c_flyTimer; //��s�\�Ȏc�莞��
	bool m_isRest = 0.0f; //��s�s�Ȃ�true

	static constexpr float c_coolTimer = 1.0f;
	float coolTimer = 0.0f; //�f����іh�~�̃N�[���^�C��

	GameObj::Suicider::CEffekseer* restEffect = nullptr;//��s�s���Ԓ��̃G�t�F�N�g
	GameObj::Suicider::CEffekseer* recoverEffect = nullptr;//��s�s�񕜂̃G�t�F�N�g
	static constexpr float c_restEfkTimer = 1.0f;
	float m_restEfkTimer = c_restEfkTimer;//�G�t�F�N�g����莞�Ԓu���ɍĐ����邽�߂̃^�C�}�[
	static constexpr float c_recoverEfkTime = 1.0f;

	float upDown = 0.0f;//�㉺�����̈ړ�

	bool hitWall = false;//�ǂɏՓ˂�����true�ɂȂ�

	CQuaternion springRot;//���炩�ȃo�l��]�̂��߂̎l����

	float flyPower = 0.0f;
	bool flying = false;

	SuicideObj::CSE* se = nullptr;//��s���̕��؂艹
};

