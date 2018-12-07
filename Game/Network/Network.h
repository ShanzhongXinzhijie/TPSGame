#pragma once

#include "ActionSender.h"
#include "NetPlayerReceiver.h"
#include "NetworkConst.h"

/*class NetJoinLeaveCaster : public IQSGameObject {
public:

	void PostUpdate()override {
		if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED || !m_needSend) { return; }

		ExitGames::Common::Hashtable _event;
		nByte send = 0;
		if (m_isJoin) {
			send++;
			if (m_first) {
				send++;
			}
		}
		_event.put(static_cast<nByte>(1),send);

		GetPhoton()->Send(enJoinLeaveEvent, _event, true);

		m_needSend = false;
	}

	//���M
	void Send(bool isJoin, bool first) {
		m_isJoin = isJoin;
		m_first = first;
		m_needSend = true;
	}

private:
	bool m_needSend;
	bool m_isJoin;
	bool m_first;
};*/

class NetWorkManager : public IGameObject{
public:
	NetWorkManager() {
		//�t�H�g��������
		GetEngine().InitPhoton(L"3d457b93-aeb7-4fb7-b958-808dd8b623a4", L"Alpha",
			[&](int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) {
				for (auto& EventAction : m_eventActionList) {
					EventAction(playerNr, eventCode, eventContentObj);
				}
			}
		);

		//���V�[�o�[�̐ݒ�
		m_netPlyReceiver.SetName(L"NetPlayerReceiver");
		AddEventAction(
			[&](int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) {
				m_netPlyReceiver.EventAction(playerNr, eventCode, eventContentObj);
			}
		);
	}	

	//Server�ɐڑ����ă��[���ɓ���
	void ConnectJoin(const wchar_t* playerName) {
		if (!GetPhoton()->GetConnected()) {
			GetPhoton()->ConnectServer(playerName);
		}
	}
	void Update()override {
		//Server�ɐڑ�������Ƀ��[���ɓ���
		if (GetPhoton()->GetState() == PhotonNetworkLogic::CONNECTED) {
			GetPhoton()->JoinRoom(L"��������", NET_MAX_PLAYER);
		}
	}

	//�C�x���g��M���Ɏ��s����֐���ǉ�
	void AddEventAction(std::function<void(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj)> func) {
		m_eventActionList.push_back(func);
	}

	NetPlayerReceiver& GetNetPlayerReceiver() {
		return m_netPlyReceiver;
	}

	//void setJoinLeaveFunc(const std::function<void(bool, bool,int)>& joinFunc) {
	//	m_joinLeaveFunc = joinFunc;
	//}

	/*void delFunc() {
		m_joinLeaveFunc = nullptr;
		m_actionFunc = nullptr;
	}

	void setActionFunc(const std::function<void(const ActionSender&, int)>& acFunc) {
		m_actionFunc = acFunc;
	}*/

private:
	//�C�x���g��M���Ɏ��s����֐�
	std::list<std::function<void(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj)>> m_eventActionList;

	NetPlayerReceiver m_netPlyReceiver;

	//std::function<void(bool, bool,int)> m_joinLeaveFunc = nullptr;
	//std::function<void(ActionSender, int)> m_actionFunc = nullptr;
};