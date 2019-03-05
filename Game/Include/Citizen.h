#pragma once
#include "DemolisherWeapon/physics/character/CCharacterController.h"
#include "CPlayer.h"
#include "InstancingCitizenColorManager.h"

class ICitizenBrain;

class Citizen : public IGameObject{
public:
	Citizen(const std::unordered_map<int, CPlayer*>& players, ICitizenBrain* moveType, unsigned int id);
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

	unsigned int GetUniqueID()const { return m_uniqueID; }
	
private:
	void playSE(const wchar_t * path);
	void Attack();

	unsigned int m_uniqueID = 0;

	//�C���X�^���V���O�p���f��
	GameObj::CInstancingModelRender m_model;
	static constexpr int InstancingNum = 1024;//�C���X�^���V���O�ő吔
	enum InsAnimNum{
		anim_walk0,
		anim_walk1,
		anim_walk2,
		anim_idle,
		anim_num,
	};
	InstancingCitizenColorManager* m_ptrCitizenColorManager[(int)anim_num * 2] = { nullptr };//�s���̐F��ς���N���X(�s���ƃ]���r�A2���p��)
	//�U���A�j���p���f��
	GameObj::CSkinModelRender m_modelAttack;
	AnimationClip m_attackAnimationClip;
	static constexpr float animInterpolateSec = 0.2f;    //�A�j���[�V������Ԏ���

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

	//�ʐM�p
	int m_lastKenzokuingCnt = INT_MIN;//�Ō���ő��������^�C�~���O
	int m_lastKenzokuingPly = -1;//�Ō���ő����������v���C���[
public:
	void SetLastKenzokuingCnt(int cnt) {
		m_lastKenzokuingCnt = cnt;
	}
	int GetLastKenzokuingCnt()const {
		return m_lastKenzokuingCnt;
	}
	void SetLastKenzokuingPly(int playerNum) {
		m_lastKenzokuingPly = playerNum;
	}
	int GetLastKenzokuingPly()const {
		return m_lastKenzokuingPly;
	}
};

