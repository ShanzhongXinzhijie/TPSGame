#include "stdafx.h"
#include "NetPlayerCaster.h"
#include "CPlayer.h"
#include "Network.h"

NetPlayerCaster::NetPlayerCaster(CPlayer* pCPlayer)
{
	m_pCPlayer = pCPlayer;
	m_isDead = m_pCPlayer->GetIsDead();
}

NetPlayerCaster::~NetPlayerCaster()
{
}

void NetPlayerCaster::PostUpdate() {
	if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) { return; }
	
	//自分自身か?
	bool isMe = false;
	if (m_pCPlayer->playerNum == GetPhoton()->GetLocalPlayerNumber()) {
		isMe = true;
	}
	
	ExitGames::Common::Hashtable _event;	
	_event.put((nByte)enFrameCount, m_cnt);//フレーム番号

	//自分自身なら実行
	if (isMe) {
		
		//毎フレーム入力状態を送信
		{
			const ActionSender& AS = m_pCPlayer->GetActionSender();
			_event.put((nByte)(enActionSender + 0), (nByte)(std::round(AS.getMovement().x*100.0f) + 100));
			_event.put((nByte)(enActionSender + 1), (nByte)(std::round(AS.getMovement().y*100.0f) + 100));
			_event.put((nByte)(enActionSender + 2), (nByte)AS.isJump());
			_event.put((nByte)(enActionSender + 3), (nByte)AS.isDash());
			_event.put((nByte)(enActionSender + 4), (nByte)AS.isShot());
			_event.put((nByte)(enActionSender + 5), (nByte)(std::round(AS.getLookVec().x*100.0f) + 100));
			_event.put((nByte)(enActionSender + 6), (nByte)(std::round(AS.getLookVec().y*100.0f) + 100));
			_event.put((nByte)(enActionSender + 7), (nByte)(std::round(AS.getLookVec().z*100.0f) + 100));
			_event.put((nByte)(enActionSender + 8), (nByte)AS.isReload());
		}
		//4Fに一回座標を送信
		if (m_cnt % 4 == 0) {
			_event.put((nByte)(enPosition + 0), (int)std::round(m_pCPlayer->getPosition().x));
			_event.put((nByte)(enPosition + 1), (int)std::round(m_pCPlayer->getPosition().y));
			_event.put((nByte)(enPosition + 2), (int)std::round(m_pCPlayer->getPosition().z));
		}

		GetPhoton()->Send(enPlayerStatus, _event);
	}

	//死んでる生きてる
	if(m_isDead != m_pCPlayer->GetIsDead()){
		m_isDead = m_pCPlayer->GetIsDead();

		//殺した！
		if (!isMe) {
			if (m_isDead) {
				//_event.put((nByte)1, (nByte)m_pCPlayer->playerNum);
				GetPhoton()->Send(enKillPlayer, _event, true, ExitGames::LoadBalancing::RaiseEventOptions().setTargetPlayers(&m_pCPlayer->playerNum, 1));
			}
		}
		else {
			//死んだ・生き返った
			_event.put((nByte)1, (nByte)m_isDead);
			//キャッシュする
			int num = GetPhoton()->GetLocalPlayerNumber();
			GetPhoton()->Send(enPlayerDeadOrAlive, ExitGames::Common::Hashtable(), true, ExitGames::LoadBalancing::RaiseEventOptions().setEventCaching(ExitGames::Lite::EventCache::REMOVE_FROM_ROOM_CACHE).setTargetPlayers(&num, 1));
			GetPhoton()->Send(enPlayerDeadOrAlive, _event, true, ExitGames::LoadBalancing::RaiseEventOptions().setEventCaching(ExitGames::Lite::EventCache::ADD_TO_ROOM_CACHE));
		}
	}

	m_cnt++;
}