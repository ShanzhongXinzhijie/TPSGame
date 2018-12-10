#pragma once
#include "DemolisherWeapon/physics/character/CCharacterController.h"
#include "CPlayer.h"

class ICitizenBrain;

class Citizen : public IGameObject{
public:
	Citizen(std::vector<CPlayer*>& players, ICitizenBrain* moveType);
	~Citizen();

	void Update() override;

	bool BatHit(CPlayer* player, CVector3 dir);

	void Kansenzyoutai();

private:
	GameObj::CSkinModelRender m_model;
	enum {
		anim_walk,
		anim_idle,
		anim_num,
	};
	AnimationClip m_animationClips[anim_num];
	static constexpr float animInterpolateSec = 0.2f;    //アニメーション補間時間

	CCharacterController charaCon;
	ICitizenBrain* mover;

	std::vector<CPlayer*>& players;

	CPlayer* ownerPlayer; 

	bool isKenzoku = false;

	SuicideObj::CCollisionObj m_collision;
};

