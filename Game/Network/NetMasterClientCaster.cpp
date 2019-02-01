#include "stdafx.h"
#include "NetMasterClientCaster.h"

NetMasterClientCaster::NetMasterClientCaster()
{
}
NetMasterClientCaster::~NetMasterClientCaster()
{
}

void NetMasterClientCaster::PostUpdate() {

	/*if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) { return; }
	if (!GetPhoton()->GetIsMasterClient()) { return; }

	//ルーム設定の送信
	if (m_isChangeSetting) {
		m_isChangeSetting = false;

		ExitGames::Common::Hashtable _event;
		_event.put((nByte)(enPosition + 2), (int)std::round(m_pCPlayer->getPosition().z));
		GetPhoton()->Send(enPlayerStatus, _event);
	}*/
}

//Ready
//ゲーム開始　seed・時間送る
//ゲーム終了  ゾンビ数送る