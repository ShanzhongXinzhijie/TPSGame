#pragma once
#include "NetworkConst.h"
#include "NetPlayerCaster.h"
#include "CPlayer.h"
#include "CitizenGene.h"
#include "../Ginger/GingerGene.h"
#include <queue>

struct OnlinePlayerStatus {
	bool m_isNoReceive = true;
	int m_cnt = INT_MIN;

	ActionSender m_actionSender;
	bool m_isUpd8Fly = false;			bool m_isFly = false;
	bool m_isUpd8Lock = false;			int m_lock = 0;
	bool m_isUpd8ActiveWeapon = false;	nByte m_activeWeapon = 0;
	bool m_isUpd8FlyTimer = false;		float m_flyTimer = 0.0f;
	bool m_isUpd8BulletCnt = false;		int m_bulletCnt[CPlayer::WEAPON_NUM] = {};
	bool m_isUpdatePos = false;			CVector3 m_pos; 
	bool m_isUpd8Velocity = false;		CVector3 m_velocity;
	bool m_isUpdateDead = false;		bool m_isDead = false;
};

class NetPlayerReceiver : public IQSGameObject
{
public:
	NetPlayerReceiver();
	~NetPlayerReceiver();

	void PreUpdate()override;
	void PostLoopUpdate()override;

	//イベント受信
	void EventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj);	
	//イベント実行
	void RunEvent(int playerNr, bool frameSkip = false);
	//プレイヤーに情報渡す
	void UpdatePlayer(int playerNr);
	//市民に情報渡す
	void UpdateCitizen();
	//神の力系
	void UpdateGodPower();
	void AddGodPowerLottery(int id, int time, int plyer);

	//プレイヤー設定
	void SetPlayer(CPlayer* pCPlayer, NetPlayerCaster* pCaster) {
		m_pCPlayer[pCPlayer->playerNum] = pCPlayer;
		m_pCaster[pCPlayer->playerNum] = pCaster;
		//この時点までの情報渡す
		UpdatePlayer(pCPlayer->playerNum);
	}
	//プレイヤー解除
	void RemovePlayer(int plyNum) {
		m_pCPlayer[plyNum] = nullptr;
		m_pCaster[plyNum] = nullptr;
		m_status[plyNum] = OnlinePlayerStatus();
		//キューをクリア
		std::queue<std::tuple<NetworkEventCode, int, ExitGames::Common::Hashtable>>().swap(m_eventContentQueue[plyNum]);
	}

	//市民設定
	void SetCitizenGene(CitizenGene* ptr) {
		m_citizenGene = ptr;
		if (m_citizenGene) {
			//この時点までの情報渡す
			UpdateCitizen();
		}
		else {
			//ステータスをクリア
			m_citizensStatus.clear();
			m_citizenPosListAvg.clear();
			m_citizenPosListSync.clear();
			m_citizenMoverSyncList.clear();
		}
	}

	//神社設定
	void SetGingerGene(GingerGene* ptr) {
		m_gingerGene = ptr;
		if (!m_gingerGene) {
			//リセット
			m_godPowerLotteryTimer.clear();
			m_godPowerLotteryList.clear();
		}
	}

private:
	void GetGodPower(int jinjyaNum, int plyNum);
	void UseGodPower(GodPowerType type, int plyNum);

	CPlayer* m_pCPlayer[NET_MAX_PLAYER + 1] = { nullptr };
	NetPlayerCaster* m_pCaster[NET_MAX_PLAYER + 1] = { nullptr };
	OnlinePlayerStatus m_status[NET_MAX_PLAYER + 1];

	CitizenGene* m_citizenGene = nullptr;
	struct CitizensStatus {
		int timeCnt = INT_MIN;
		int plyNum = -1;
		CVector3 pos;
	};
	std::unordered_map<int, CitizensStatus> m_citizensStatus;
	std::list<std::pair<int, CVector3>> m_citizenPosListAvg;
	std::list<std::tuple<int, CVector3, int, int>> m_citizenPosListSync;
	std::list<std::tuple<int, CVector3, int, int, CVector3>> m_citizenMoverSyncList;

	GingerGene* m_gingerGene = nullptr;
	std::unordered_map<int, int>				 m_godPowerLotteryTimer;//神社ID,時間
	std::unordered_map<int, std::pair<int, int>> m_godPowerLotteryList; //神社ID,<時間,プレイヤーID>

	std::queue<std::tuple<NetworkEventCode, int, ExitGames::Common::Hashtable>> m_eventContentQueue[NET_MAX_PLAYER + 1];
};

