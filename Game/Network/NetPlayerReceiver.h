#pragma once
#include "NetworkConst.h"
#include"../Include/CPlayer.h"

struct OnlinePlayerStatus {
	ActionSender m_actionSender;
	CVector3 m_pos;
	bool m_isDead = false;
};

class NetPlayerReceiver : public IQSGameObject
{
public:
	NetPlayerReceiver();
	~NetPlayerReceiver();

	void EventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj);	

	void SetPlayer(CPlayer* pCPlayer) {
		m_pCPlayer[pCPlayer->playerNum] = pCPlayer;

		//î•ñ“n‚·
	}
	void RemovePlayer(int plyNum) {
		m_pCPlayer[plyNum] = nullptr;
		m_status[plyNum] = OnlinePlayerStatus();
	}

	void PreUpdate()override;

private:
	CPlayer* m_pCPlayer[NET_MAX_PLAYER+1] = { nullptr };
	OnlinePlayerStatus m_status[NET_MAX_PLAYER+1];
};

