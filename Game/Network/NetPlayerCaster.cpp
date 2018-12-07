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
	
	bool isMe = false;
	if (m_pCPlayer->playerNum == GetPhoton()->GetLocalPlayerNumber()) {
		isMe = true;
	}

	//�������g�Ȃ���s
	if (isMe) {
		//���t���[�����͏�Ԃ𑗐M
		{
			ExitGames::Common::Hashtable _event;
			const ActionSender& AS = m_pCPlayer->GetActionSender();
			_event.put((nByte)1, (int)std::round(AS.getMovement().x*100.0f));
			_event.put((nByte)2, (int)std::round(AS.getMovement().y*100.0f));
			_event.put((nByte)3, (nByte)AS.isJump());
			_event.put((nByte)4, (nByte)AS.isDash());
			_event.put((nByte)5, (nByte)AS.isShot());
			_event.put((nByte)6, (int)std::round(AS.getLookVec().x*100.0f));
			_event.put((nByte)7, (int)std::round(AS.getLookVec().y*100.0f));
			_event.put((nByte)8, (int)std::round(AS.getLookVec().z*100.0f));

			GetPhoton()->Send(enActionSender, _event);
		}
		//4F�Ɉ����W�𑗐M
		if (m_cnt % 4 == 0) {
			ExitGames::Common::Hashtable _event;
			_event.put((nByte)1, (int)std::round(m_pCPlayer->getPosition().x));
			_event.put((nByte)2, (int)std::round(m_pCPlayer->getPosition().y));
			_event.put((nByte)3, (int)std::round(m_pCPlayer->getPosition().z));

			GetPhoton()->Send(enPosition, _event);
		}
	}

	//���񂾁E�����Ԃ����^�C�~���O�ő��M
	if(m_isDead != m_pCPlayer->GetIsDead()){
		m_isDead = m_pCPlayer->GetIsDead();

		//�L���b�V������
		int num = GetPhoton()->GetLocalPlayerNumber();
		GetPhoton()->Send(enIsDead, ExitGames::Common::Hashtable(), true, ExitGames::LoadBalancing::RaiseEventOptions().setEventCaching(ExitGames::Lite::EventCache::REMOVE_FROM_ROOM_CACHE).setTargetPlayers(&num, 1));
		GetPhoton()->Send(enIsDead, (nByte)m_isDead, true, ExitGames::LoadBalancing::RaiseEventOptions().setEventCaching(ExitGames::Lite::EventCache::ADD_TO_ROOM_CACHE));
	}

	m_cnt++;
}