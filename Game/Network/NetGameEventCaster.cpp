#include "stdafx.h"
#include "NetGameEventCaster.h"

NetGameEventCaster::NetGameEventCaster()//ConfirmPlayers* pCP)
{
	//m_pConfirmPlayers = pCP;
}
NetGameEventCaster::~NetGameEventCaster()
{
}

void NetGameEventCaster::SendReady(bool ready) {
	if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) { return; }

	//Ready�̑��M
	GetPhoton()->SetPlayerProperty(PLAYER_READY, (nByte)ready);
}

void NetGameEventCaster::SendRoomSetting_TimeLimit(float timelimit) {
	if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) { return; }
	if (!GetPhoton()->GetIsMasterClient()) { return; }

	//�������Ԃ̑��M
	GetPhoton()->SetRoomProperty(TIME_LIMIT, timelimit);
}

void NetGameEventCaster::SendRoomSetting_CitizenCount(int count) {
	if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) { return; }
	if (!GetPhoton()->GetIsMasterClient()) { return; }

	//�s���l���̑��M
	GetPhoton()->SetRoomProperty(CITIZEN_COUNT, count);
}

void NetGameEventCaster::SendGameStart(int seed) {
	if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) { return; }
	if (!GetPhoton()->GetIsMasterClient()) { return; }

	//���[�������
	GetPhoton()->GetMutableRoom().setIsVisible(false);
	GetPhoton()->GetMutableRoom().setIsOpen(false);

	//�Q�[���J�n
	ExitGames::Common::Hashtable _event;
	_event.put<nByte, int>((nByte)(enSeed), seed);
	_event.put<nByte, int>((nByte)(enSeverTime), GetPhoton()->GetSeverTime_ms());

	GetPhoton()->Send(enMasterClientStartGame, _event, true);
}

void NetGameEventCaster::SendGameEnd(int zombieCnt[NET_MAX_PLAYER + 1]) {
	if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) { return; }
	if (!GetPhoton()->GetIsMasterClient()) { return; }

	//�Q�[���I��
	ExitGames::Common::Hashtable _event;
	for (int i = 0; i < NET_MAX_PLAYER + 1; i++) {
		_event.put((nByte)(i), zombieCnt[i]);
	}

	GetPhoton()->Send(enMasterClientEndGame, _event, true);

	//���[�����J����
	GetPhoton()->GetMutableRoom().setIsVisible(true);
	GetPhoton()->GetMutableRoom().setIsOpen(true);
}