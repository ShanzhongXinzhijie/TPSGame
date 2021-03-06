#pragma once
#include "DemolisherWeapon/physics/character/CCharacterController.h"
#include "CPlayer.h"
#include "../Network/NetPlayerCaster.h"
#include "MiniHPbar.h"
#include "IDamagable.h"

class ICitizenBrain;
class Bullet;

class Citizen : public IGameObject, public IDamagable{
public:
	Citizen(const std::unordered_map<int, CPlayer*>& players, ICitizenBrain* moveType, unsigned int id);
	~Citizen();

	void Update() override;
	
	void setPos(const CVector3& pos) {
		charaCon.SetPosition(pos);
	}

	const CVector3& getPos() const{
		return charaCon.GetPosition();
	}

	bool damage(const CVector3& vec, unsigned int damage,
				const Team* team = nullptr, const CPlayer* = nullptr) override;
	
	void ChangeToKenzoku(const CPlayer* player);//眷属化

	void Kansenzyoutai();

	void setUpdateDelay(unsigned char flame, unsigned char flamePeriod) {
		updateFlame = flame;
		nowFlame = flamePeriod;
	}

	unsigned int GetUniqueID()const { return m_uniqueID; }

	const Team* GetOwnerTeam()const{
		return ownerTeam;
	}

	void Death();
	bool GetIsDead()const;
	
private:
	void playSE(const wchar_t * path);
	void Attack();

	unsigned int m_uniqueID = 0;
	bool m_isDead = false;

	//インスタンシング用モデル
	GameObj::CInstancingModelRender m_model;
	static constexpr int InstancingNum = 1024;//インスタンシング最大数
	enum InsAnimNum{
		anim_walk0,
		anim_walk1,
		anim_walk2,
		anim_idle,
		anim_num,
	};
	CVector4 m_color;//チームカラー
	//攻撃アニメ用モデル
	GameObj::CSkinModelRender m_modelAttack;
	AnimationClip m_attackAnimationClip;
	static constexpr float animInterpolateSec = 0.2f;    //アニメーション補間時間

	bool attacking = false;
	const unsigned int attackPower = 200;

	const unsigned int maxHp = 250;
	unsigned int m_hp = maxHp;
	MiniHPbar miniHpbar;

	CCharacterController charaCon;
	ICitizenBrain* mover;
	
	const std::unordered_map<int, CPlayer*>& playersMap;

	Team* ownerTeam = nullptr; 

	bool isKenzoku = false;

	SuicideObj::CCollisionObj m_collision;

	float deltaTime = 0;

	unsigned char nowFlame = 0;
	unsigned char updateFlame = 0;

//////通信用
	int m_netCnt = INT_MIN;
	//位置同期系
	bool m_isSend = false, m_sendType_Avg = false;
	int m_targetply = -1, m_targetCnt = INT_MIN;
	//眷属化系
	int m_lastKenzokuingCnt = INT_MIN;//最後に眷属化したタイミング
	int m_lastKenzokuingPly = -1;//最後に眷属化させたプレイヤー
	//Mover系
	bool m_isSendMover = false;
	int m_moverPly = -1, m_moverCnt = INT_MIN;
public:
	int GetNetCnt()const { return m_netCnt; }
	
	//一同期形
	void SetIsSend(bool issend) { m_isSend = issend; }
	bool GetIsSend()const { return m_isSend; }
	void SetIsAvg(bool is) { m_sendType_Avg = is; }
	bool GetIsAvg()const { return m_sendType_Avg; }

	void SetTargetPly(int playerNum) { m_targetply = playerNum; }
	int GetTargetPly()const { return m_targetply; }
	void SetTargetCnt(int cnt) {
		m_targetCnt = cnt; 
		if (m_targetCnt > m_netCnt) {
			m_netCnt = m_targetCnt;
		}
	}
	int GetTargetCnt()const { return m_targetCnt; }
	
	//眷属化系
	void SetLastKenzokuingCnt(int cnt) {
		m_lastKenzokuingCnt = cnt;
		if (m_lastKenzokuingCnt > m_netCnt) {
			m_netCnt = m_lastKenzokuingCnt;
		}
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

	//Mover系
	void SetIsSendMover(bool issend) { m_isSendMover = issend; }
	bool GetIsSendMover()const { return m_isSendMover; }

	void SetSyncMoverPly(int playerNum) { m_moverPly = playerNum; }
	int GetSyncMoverPly()const { return m_moverPly; }
	void SetSyncMoverCnt(int cnt) {
		m_moverCnt = cnt;
		if (m_moverCnt > m_netCnt) {
			m_netCnt = m_moverCnt;
		}
	}
	int GetSyncMoverCnt()const { return m_moverCnt; }

	const CVector3& GetMoverNetVec()const;
	void SetMoverNetVec(const CVector3&);
};

