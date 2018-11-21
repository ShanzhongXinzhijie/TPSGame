#pragma once
#include "DemolisherWeapon/physics/character/CCharacterController.h"

class ICitizenMover;

class Citizen : public IGameObject{
public:
	Citizen(ICitizenMover* moveType);
	~Citizen();

	void Update() override;

private:
	GameObj::CSkinModelRender m_model;
	enum {
		anim_walk,
		anim_idle,
		anim_num,
	};
	AnimationClip m_animationClips[anim_num];
	const float animInterpolateSec = 0.2f;        //アニメーション補間時間

	CCharacterController charaCon;
	ICitizenMover* mover;
};

