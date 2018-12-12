#pragma once

#include "ActionSender.h"
#include "NetPlayerReceiver.h"
#include "NetworkConst.h"

class NetWorkManager : public IGameObject{
public:
	NetWorkManager() {
		//�t�H�g��������
		GetEngine().InitPhoton(L"3d457b93-aeb7-4fb7-b958-808dd8b623a4", L"Alpha2b",
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

private:
	//�C�x���g��M���Ɏ��s����֐�
	std::list<std::function<void(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj)>> m_eventActionList;

	NetPlayerReceiver m_netPlyReceiver;
};