#pragma once
#include "NetworkConst.h"
#include "NetPlayerCaster.h"
#include"../Include/CPlayer.h"
#include <queue>

struct OnlinePlayerStatus {
	bool m_isNoReceive = true;
	int m_cnt = INT_MIN;

	ActionSender m_actionSender;
	bool m_isUpdatePos = false;		CVector3 m_pos; 
	bool m_isUpdateDead = false;	bool m_isDead = false;
};

class NetPlayerReceiver : public IQSGameObject
{
public:
	NetPlayerReceiver();
	~NetPlayerReceiver();

	void PreUpdate()override;

	//イベント受信
	void EventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj);	
	//イベント実行
	void RunEvent(int playerNr);
	//プレイヤーに情報渡す
	void UpdatePlayer(int playerNr);

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

private:
	CPlayer* m_pCPlayer[NET_MAX_PLAYER + 1] = { nullptr };
	NetPlayerCaster* m_pCaster[NET_MAX_PLAYER + 1] = { nullptr };
	OnlinePlayerStatus m_status[NET_MAX_PLAYER + 1];

	std::queue<std::tuple<NetworkEventCode, int, ExitGames::Common::Hashtable>> m_eventContentQueue[NET_MAX_PLAYER + 1];
};

