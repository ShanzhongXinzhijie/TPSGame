#include "stdafx.h"
#include "NetGameEventReceiver.h"

NetGameEventReceiver::NetGameEventReceiver()
{
}

NetGameEventReceiver::~NetGameEventReceiver()
{
}

void NetGameEventReceiver::EventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) {
	if (eventCode > enMasterClientEventStart && eventCode < enMasterClientEventEnd) {
		if (eventCode == enMasterClientStartGame) {
			//ゲーム開始
			m_gameState = enGameStart;
			
			ExitGames::Common::Hashtable eventContent = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContentObj).getDataCopy();
			m_gameStartData.m_seed = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enSeed))))->getDataCopy();
			m_gameStartData.m_startTime = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enSeverTime))))->getDataCopy();
		}
		if (eventCode == enMasterClientEndGame) {
			//ゲーム終わり
			m_gameState = enGameEnd;

			ExitGames::Common::Hashtable eventContent = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContentObj).getDataCopy();
			for (int i = 0; i < NET_MAX_PLAYER + 1; i++) {
				m_gameEndData.m_zombieCnt[i] = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(i))))->getDataCopy();
			}
		}
	}
}