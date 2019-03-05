#include "stdafx.h"
#include "NetPlayerCaster.h"
#include "CPlayer.h"
#include "Citizen.h"
#include "Network.h"

NetPlayerCaster::NetPlayerCaster(CPlayer* pCPlayer)
{
	m_pCPlayer = pCPlayer;
	m_pCPlayer->SetNetCaster(this);
	m_isDead = m_pCPlayer->GetIsDead();
}

NetPlayerCaster::~NetPlayerCaster()
{
}

void NetPlayerCaster::PostUpdate() {
	if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) { return; }
	
	//�������g��?
	bool isMe = false;
	if (m_pCPlayer->playerNum == GetPhoton()->GetLocalPlayerNumber()) {
		isMe = true;
	}	

	//�������g�Ȃ���s
	if (isMe) {
	//4F�Ɉ�񑗐M
	if (m_cnt % 4 == 0) {		
		ExitGames::Common::Hashtable _event;

		//�t���[���ԍ�
		_event.put((nByte)enFrameCount, m_cnt);

		//�A�i���O����
		const ActionSender& AS = m_pCPlayer->GetActionSender();
		_event.put((nByte)(enActionSender + 0), (nByte)(std::round(AS.getMovement().x*100.0f) + 100));
		_event.put((nByte)(enActionSender + 1), (nByte)(std::round(AS.getMovement().y*100.0f) + 100));
		_event.put((nByte)(enActionSender + 2), (nByte)(std::round(AS.getLookVec().x*100.0f) + 100));
		_event.put((nByte)(enActionSender + 3), (nByte)(std::round(AS.getLookVec().y*100.0f) + 100));
		_event.put((nByte)(enActionSender + 4), (nByte)(std::round(AS.getLookVec().z*100.0f) + 100));
		//�{�^������
		nByte bottuns = 0;
		if (AS.isJump()) { bottuns = bottuns | 0b1; }
		if (AS.isDash()) { bottuns = bottuns | 0b10; }
		if (AS.isShot()) { bottuns = bottuns | 0b100; }
		if (AS.isReload()) { bottuns = bottuns | 0b1000; }
		_event.put((nByte)(enActionSender + 5), (nByte)bottuns);

		//���W
		_event.put((nByte)(enPosition + 0), (int)std::round(m_pCPlayer->getPosition().x));
		_event.put((nByte)(enPosition + 1), (int)std::round(m_pCPlayer->getPosition().y));
		_event.put((nByte)(enPosition + 2), (int)std::round(m_pCPlayer->getPosition().z));

		//���M
		GetPhoton()->Send(enPlayerStatus, _event);		
	}
	//12F�Ɉ�񑗐M
	if (m_cnt % 12 == 0) {
		//�ő����𑗐M
		if(m_sendKenzokuList.size() > 0){
			ExitGames::Common::Hashtable _event;
			_event.put((nByte)enFrameCount, m_cnt);//�t���[���ԍ�

			_event.put((nByte)1, (int)m_sendKenzokuList.size());//����
			int i = 2;
			for (auto& K : m_sendKenzokuList) {
				_event.put(i, K.first);  i++;//ID
				_event.put(i, K.second); i++;//����
			}
			m_sendKenzokuList.clear();

			//���M
			GetPhoton()->Send(enKenzoku, _event, true);
		}
	}
	}

	//����ł鐶���Ă�
	if(m_isDead != m_pCPlayer->GetIsDead()){
		m_isDead = m_pCPlayer->GetIsDead();

		ExitGames::Common::Hashtable _event;
		_event.put((nByte)enFrameCount, m_cnt);//�t���[���ԍ�

		//�E�����I
		if (!isMe) {
			if (m_isDead) {
				//�E��������ɑ���
				GetPhoton()->Send(enKillPlayer, _event, true, ExitGames::LoadBalancing::RaiseEventOptions().setTargetPlayers(&m_pCPlayer->playerNum, 1));
			}
		}
		else {
			//���񂾁E�����Ԃ���
			_event.put((nByte)1, (nByte)m_isDead);
			GetPhoton()->Send(enPlayerDeadOrAlive, _event, true);
			
			//�L���b�V������
			//int num = GetPhoton()->GetLocalPlayerNumber();
			//GetPhoton()->Send(enPlayerDeadOrAlive, ExitGames::Common::Hashtable(), true, ExitGames::LoadBalancing::RaiseEventOptions().setEventCaching(ExitGames::Lite::EventCache::REMOVE_FROM_ROOM_CACHE).setTargetPlayers(&num, 1));
			//GetPhoton()->Send(enPlayerDeadOrAlive, _event, true, ExitGames::LoadBalancing::RaiseEventOptions().setEventCaching(ExitGames::Lite::EventCache::ADD_TO_ROOM_CACHE));
		}
	}

	m_cnt++;
}

void NetPlayerCaster::SendNewKenzoku(::Citizen* pkenzoku) {
	m_sendKenzokuList.emplace_back(std::make_pair((int)pkenzoku->GetUniqueID(), m_cnt));
	pkenzoku->SetLastKenzokuingCnt(m_cnt);
	pkenzoku->SetLastKenzokuingPly(m_cnt);
}
