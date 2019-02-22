#pragma once
#include "DemolisherWeapon/physics/character/CCharacterController.h"
#include "CPlayer.h"
#include "InstancingCitizenColorManager.h"

class ICitizenBrain;

class Citizen : public IGameObject{
public:
	Citizen(const std::unordered_map<int, CPlayer*>& players, ICitizenBrain* moveType);
	~Citizen();

	void Update() override;
	void PostLoopUpdate() override;

	void setPos(const CVector3& pos) {
		charaCon.SetPosition(pos);
	}

	CVector3 getPos() const{
		return charaCon.GetPosition();
	}

	bool BatHit(CPlayer* player, CVector3 dir);

	void Kansenzyoutai();

	void setUpdateDelay(unsigned char flame, unsigned char flamePeriod) {
		updateFlame = flame;
		nowFlame = flamePeriod;
	}

private:
	void playSE(const wchar_t * path);
	void Attack();

	GameObj::CInstancingModelRender m_model;
	GameObj::CSkinModelRender m_modelAttack;
	static constexpr float InstancingNum = 1024;
	enum {
		anim_walk,
		anim_idle,
		anim_attack,
		anim_num,
	};
	AnimationClip m_animationClips[anim_num];
	static constexpr float animInterpolateSec = 0.2f;    //アニメーション補間時間
	InstancingCitizenColorManager* m_ptrCitizenColorManager[4] = { nullptr };

	bool attacking = false;

	CCharacterController charaCon;
	ICitizenBrain* mover;
	
	const std::unordered_map<int, CPlayer*>& playersMap;

	Team* ownerTeam = nullptr; 

	bool isKenzoku = false;

	SuicideObj::CCollisionObj m_collision;

	float deltaTime = 0;

	unsigned char nowFlame = 0;
	unsigned char updateFlame = 0;
};

