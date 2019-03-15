#pragma once

#include "ActionSender.h"
#include "NetPlayerReceiver.h"
#include "NetGameEventReceiver.h"
#include "NetworkConst.h"

static const wchar_t NETWORK_VER[] = L"Alpha15";

class NetWorkManager : public IGameObject{
public:
	NetWorkManager() {
		//�t�H�g��������
		GetEngine().InitPhoton(L"3d457b93-aeb7-4fb7-b958-808dd8b623a4", NETWORK_VER,
			//�C�x���g��M���Ɏ��s����֐����Z�b�g
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

		AddEventAction(
			[&](int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) {
			m_netEventReceiver.EventAction(playerNr, eventCode, eventContentObj);
		}
		);

		//�G���[����
		GetEngine().GetPhoton()->SetErrorReturnAction([&](int errorCode, const wchar_t* errorString, const wchar_t* errorPoint) { ErrorReceiver(errorCode, errorString, errorPoint); });
	}	

	//Server�ɐڑ�
	void Connect(const wchar_t* playerName) {
		if (!GetPhoton()->GetConnected()) {
			GetPhoton()->ConnectServer(playerName);
			m_errorMes.clear();
		}
	}
	void Update()override {
		//Server�ɐڑ�������Ƀ��[���ɓ���
		if (GetPhoton()->GetState() == PhotonNetworkLogic::CONNECTED) {
			GetPhoton()->JoinRoom(L"��������", NET_MAX_PLAYER);
		}
	}
	//Server����ؒf
	void Disconnect() {
		GetPhoton()->DisconnectServer();
	}

	//�C�x���g��M���Ɏ��s����֐���ǉ�
	void AddEventAction(std::function<void(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj)> func) {
		m_eventActionList.push_back(func);
	}

	NetPlayerReceiver& GetNetPlayerReceiver() {
		return m_netPlyReceiver;
	}
	NetGameEventReceiver& GetNetEventReceiver() {
		return m_netEventReceiver;
	}

	//�ŐV�̃G���[���b�Z�[�W�擾
	const wchar_t* GetErrorMessage()const { return m_errorMes.c_str(); }

	//�G���[�o�Ă邩�擾
	bool GetIsError()const { return !m_errorMes.empty(); }

private:

	//�G���[�������̏���
	void ErrorReceiver(int errorCode, const wchar_t* errorString, const wchar_t* errorPoint) {
		m_errorMes.clear();

		wchar_t str[16];
		swprintf_s(str, L"%d:", errorCode);
		m_errorMes += str;

		m_errorMes += errorString;

		m_errorMes += L"(";
		m_errorMes += errorPoint;
		m_errorMes += L")";

		if (errorCode == ExitGames::LoadBalancing::ErrorCode::GAME_CLOSED) {
			m_errorMes += L"\n�����͂��܂��Ă�I";
		}
	}

	std::wstring m_errorMes;

private:
	//�C�x���g��M���Ɏ��s����֐�
	std::list<std::function<void(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj)>> m_eventActionList;

	NetPlayerReceiver m_netPlyReceiver;
	NetGameEventReceiver m_netEventReceiver;
};