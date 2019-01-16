#pragma once
#include "DemolisherWeapon/physics/character/CCharacterController.h"
#include "CPlayer.h"

class ICitizenBrain;

class Citizen : public IGameObject{
public:
	Citizen(const std::unordered_map<int, CPlayer*>& players, ICitizenBrain* moveType);
	~Citizen();

	void Update() override;

	void setPos(const CVector3& pos) {
		charaCon.SetPosition(pos);
	}

	bool BatHit(CPlayer* player, CVector3 dir);

	void Kansenzyoutai();

private:
	void Attack();

	GameObj::CSkinModelRender m_model;
	enum {
		anim_walk,
		anim_idle,
		anim_attack,
		anim_num,
	};
	AnimationClip m_animationClips[anim_num];
	static constexpr float animInterpolateSec = 0.2f;    //アニメーション補間時間

	bool attacking = false;

	CCharacterController charaCon;
	ICitizenBrain* mover;

	const std::unordered_map<int, CPlayer*>& playersMap;

	Team* ownerTeam = nullptr; 

	bool isKenzoku = false;

	SuicideObj::CCollisionObj m_collision;
};

