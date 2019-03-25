#pragma once
#include "Walker.h"
class FlyWalker : public Walker {
public:
	FlyWalker();
	~FlyWalker();

	void fly(bool isFly, const CVector3& velocity = {}, const CVector2& move = {}, float power = 0.0f);
	void flyStop();

	void rest();
	void restStop();

	CQuaternion getRotation() const;

	float getFlyTimer() const{
		return flyTimer;
	}
	float getFlyTimerMax() const{
		return c_flyTimer;
	}

	float getCoolTimer() const {
		return coolTimer;
	}

	bool isFlying()const {
		return flying;
	}

	bool isHitWall() const {
		return hitWall;
	}

	bool isRest() const {
		return m_isRest;
	}

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

	void Update();

	static constexpr float c_flyTimer = 5.0f;
private:
	bool m_isLocalUser = true;

	float flyTimer = c_flyTimer; //��s�\�Ȏc�莞��
	bool m_isRest = 0.0f; //��s�s��

	static constexpr float c_coolTimer = 1.0f;
	float coolTimer = 0.0f; //�f����іh�~�̃N�[���^�C��

	GameObj::Suicider::CEffekseer* restEffect = nullptr;
	GameObj::Suicider::CEffekseer* recoverEffect = nullptr;
	static constexpr float c_restEfkTimer = 1.0f;
	float m_restEfkTimer = c_restEfkTimer;
	static constexpr float c_recoverEfkTime = 1.0f;

	float upDown = 0.0f;

	bool hitWall = false;

	CQuaternion springRot;

	float flyPower = 0.0f;
	bool flying = false;

	SuicideObj::CSE* se = nullptr;
};

