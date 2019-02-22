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

	//インスタンシング用モデル
	GameObj::CInstancingModelRender m_model;
	static constexpr int InstancingNum = 1024;//インスタンシング最大数
	enum InsAnimNum{
		anim_walk,
		anim_idle,
		anim_num,
	};
	InstancingCitizenColorManager* m_ptrCitizenColorManager[(int)anim_num * 2] = { nullptr };//市民の色を変えるクラス(市民とゾンビ、2つ分用意)
	//攻撃アニメ用モデル
	GameObj::CSkinModelRender m_modelAttack;
	AnimationClip m_attackAnimationClip;
	static constexpr float animInterpolateSec = 0.2f;    //アニメーション補間時間

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

