#pragma once
#include "DemolisherWeapon/physics/character/CCharacterController.h"
#include "CPlayer.h"

class ICitizenBrain;

class Citizen : public IGameObject{
public:
	Citizen(std::unordered_map<int, CPlayer*>& players, ICitizenBrain* moveType);
	~Citizen();

	void Update() override;

	void setPos(const CVector3& pos) {
		charaCon.SetPosition(pos);
	}

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

	std::unordered_map<int, CPlayer*>& playersMap;

	Team* ownerTeam = nullptr; 

	bool isKenzoku = false;

	SuicideObj::CCollisionObj m_collision;
};

