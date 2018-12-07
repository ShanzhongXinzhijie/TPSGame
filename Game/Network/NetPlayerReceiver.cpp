#include "stdafx.h"
#include "NetPlayerReceiver.h"


NetPlayerReceiver::NetPlayerReceiver()
{
}
NetPlayerReceiver::~NetPlayerReceiver()
{
}

void NetPlayerReceiver::EventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) {
	
	switch (eventCode)
	{
	case enActionSender:
	{
		ExitGames::Common::Hashtable eventContent = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContentObj).getDataCopy();

		if (eventContent.getValue((nByte)8)) {
			m_status[playerNr].m_actionSender = ActionSender(
				{
					(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)1)))->getDataCopy() / 100.0f,
					(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)2)))->getDataCopy() / 100.0f
				},
				((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)3)))->getDataCopy(),
				((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)4)))->getDataCopy(),
				{
					(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)6)))->getDataCopy() / 100.0f,
					(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)7)))->getDataCopy() / 100.0f,
					(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)8)))->getDataCopy() / 100.0f
				},
				((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)5)))->getDataCopy()
			);
		}
	}
		break;
	case enPosition:
	{
		ExitGames::Common::Hashtable eventContent = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContentObj).getDataCopy();

		if (eventContent.getValue((nByte)3)) {
			m_status[playerNr].m_pos.Set(
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)1)))->getDataCopy(),
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)2)))->getDataCopy(),
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)3)))->getDataCopy()
			);
		}
	}
		break;
	case enIsDead:
	{
		nByte isDead = ExitGames::Common::ValueObject<nByte>(eventContentObj).getDataCopy();
		m_status[playerNr].m_isDead = isDead;
	}
		break;
	default:
		break;
	}
}

//更新されたstatsのみ適応してやる
void NetPlayerReceiver::PreUpdate() {
	int i = 0;
	for (auto& P : m_pCPlayer) {
		//プレイヤーに情報渡す
		if (P) {
			P->sendAction(m_status[i].m_actionSender);
		}
		i++;
	}
}