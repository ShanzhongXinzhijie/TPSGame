#pragma once

#include "ActionSender.h"
#include "NetPlayerReceiver.h"
#include "NetworkConst.h"

class NetWorkManager : public IGameObject{
public:
	NetWorkManager() {
		//フォトン初期化
		GetEngine().InitPhoton(L"3d457b93-aeb7-4fb7-b958-808dd8b623a4", L"Alpha2b",
			[&](int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) {
				for (auto& EventAction : m_eventActionList) {
					EventAction(playerNr, eventCode, eventContentObj);
				}
			}
		);

		//レシーバーの設定
		m_netPlyReceiver.SetName(L"NetPlayerReceiver");
		AddEventAction(
			[&](int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) {
				m_netPlyReceiver.EventAction(playerNr, eventCode, eventContentObj);
			}
		);
	}	

	//Serverに接続してルームに入る
	void ConnectJoin(const wchar_t* playerName) {
		if (!GetPhoton()->GetConnected()) {
			GetPhoton()->ConnectServer(playerName);
		}
	}
	void Update()override {
		//Serverに接続した後にルームに入る
		if (GetPhoton()->GetState() == PhotonNetworkLogic::CONNECTED) {
			GetPhoton()->JoinRoom(L"ああああ", NET_MAX_PLAYER);
		}
	}

	//イベント受信時に実行する関数を追加
	void AddEventAction(std::function<void(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj)> func) {
		m_eventActionList.push_back(func);
	}

	NetPlayerReceiver& GetNetPlayerReceiver() {
		return m_netPlyReceiver;
	}

private:
	//イベント受信時に実行する関数
	std::list<std::function<void(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj)>> m_eventActionList;

	NetPlayerReceiver m_netPlyReceiver;
};