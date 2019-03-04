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

	//自分自身なら実行
	if (isMe) {
	//4Fに一回送信
	if (m_cnt % 4 == 0) {
		ExitGames::Common::Hashtable _event;
		
		//フレーム番号
		_event.put((nByte)enFrameCount, m_cnt);
		
		//アナログ入力
		const ActionSender& AS = m_pCPlayer->GetActionSender();
		_event.put((nByte)(enActionSender + 0), (nByte)(std::round(AS.getMovement().x*100.0f) + 100));
		_event.put((nByte)(enActionSender + 1), (nByte)(std::round(AS.getMovement().y*100.0f) + 100));
		_event.put((nByte)(enActionSender + 2), (nByte)(std::round(AS.getLookVec().x*100.0f) + 100));
		_event.put((nByte)(enActionSender + 3), (nByte)(std::round(AS.getLookVec().y*100.0f) + 100));
		_event.put((nByte)(enActionSender + 4), (nByte)(std::round(AS.getLookVec().z*100.0f) + 100));
		//ボタン入力
		nByte bottuns = 0;
		if (AS.isJump())	{ bottuns = bottuns | 0b1; }
		if (AS.isDash())	{ bottuns = bottuns | 0b10; }
		if (AS.isShot())	{ bottuns = bottuns | 0b100; }
		if (AS.isReload())	{ bottuns = bottuns | 0b1000; }
		_event.put((nByte)(enActionSender + 5), (nByte)bottuns);

		//座標
		_event.put((nByte)(enPosition + 0), (int)std::round(m_pCPlayer->getPosition().x));
		_event.put((nByte)(enPosition + 1), (int)std::round(m_pCPlayer->getPosition().y));
		_event.put((nByte)(enPosition + 2), (int)std::round(m_pCPlayer->getPosition().z));		

		//送信
		GetPhoton()->Send(enPlayerStatus, _event);
	}
	}

	//死んでる生きてる
	if(m_isDead != m_pCPlayer->GetIsDead()){
		m_isDead = m_pCPlayer->GetIsDead();

		ExitGames::Common::Hashtable _event;
		_event.put((nByte)enFrameCount, m_cnt);//フレーム番号

		//殺した！
		if (!isMe) {
			if (m_isDead) {
				//殺した相手に送る
				GetPhoton()->Send(enKillPlayer, _event, true, ExitGames::LoadBalancing::RaiseEventOptions().setTargetPlayers(&m_pCPlayer->playerNum, 1));
			}
		}
		else {
			//死んだ・生き返った
			_event.put((nByte)1, (nByte)m_isDead);
			GetPhoton()->Send(enPlayerDeadOrAlive, _event, true);
			
			//キャッシュする
			//int num = GetPhoton()->GetLocalPlayerNumber();
			//GetPhoton()->Send(enPlayerDeadOrAlive, ExitGames::Common::Hashtable(), true, ExitGames::LoadBalancing::RaiseEventOptions().setEventCaching(ExitGames::Lite::EventCache::REMOVE_FROM_ROOM_CACHE).setTargetPlayers(&num, 1));
			//GetPhoton()->Send(enPlayerDeadOrAlive, _event, true, ExitGames::LoadBalancing::RaiseEventOptions().setEventCaching(ExitGames::Lite::EventCache::ADD_TO_ROOM_CACHE));
		}
	}

	m_cnt++;
}