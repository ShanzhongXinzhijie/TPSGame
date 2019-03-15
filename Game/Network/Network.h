#pragma once

#include "ActionSender.h"
#include "NetPlayerReceiver.h"
#include "NetGameEventReceiver.h"
#include "NetworkConst.h"

static const wchar_t NETWORK_VER[] = L"Alpha15";

class NetWorkManager : public IGameObject{
public:
	NetWorkManager() {
		//フォトン初期化
		GetEngine().InitPhoton(L"3d457b93-aeb7-4fb7-b958-808dd8b623a4", NETWORK_VER,
			//イベント受信時に実行する関数をセット
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

		AddEventAction(
			[&](int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) {
			m_netEventReceiver.EventAction(playerNr, eventCode, eventContentObj);
		}
		);

		//エラー処理
		GetEngine().GetPhoton()->SetErrorReturnAction([&](int errorCode, const wchar_t* errorString, const wchar_t* errorPoint) { ErrorReceiver(errorCode, errorString, errorPoint); });
	}	

	//Serverに接続
	void Connect(const wchar_t* playerName) {
		if (!GetPhoton()->GetConnected()) {
			GetPhoton()->ConnectServer(playerName);
			m_errorMes.clear();
		}
	}
	void Update()override {
		//Serverに接続した後にルームに入る
		if (GetPhoton()->GetState() == PhotonNetworkLogic::CONNECTED) {
			GetPhoton()->JoinRoom(L"ああああ", NET_MAX_PLAYER);
		}
	}
	//Serverから切断
	void Disconnect() {
		GetPhoton()->DisconnectServer();
	}

	//イベント受信時に実行する関数を追加
	void AddEventAction(std::function<void(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj)> func) {
		m_eventActionList.push_back(func);
	}

	NetPlayerReceiver& GetNetPlayerReceiver() {
		return m_netPlyReceiver;
	}
	NetGameEventReceiver& GetNetEventReceiver() {
		return m_netEventReceiver;
	}

	//最新のエラーメッセージ取得
	const wchar_t* GetErrorMessage()const { return m_errorMes.c_str(); }

	//エラー出てるか取得
	bool GetIsError()const { return !m_errorMes.empty(); }

private:

	//エラー発生時の処理
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
			m_errorMes += L"\nもうはじまってる！";
		}
	}

	std::wstring m_errorMes;

private:
	//イベント受信時に実行する関数
	std::list<std::function<void(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj)>> m_eventActionList;

	NetPlayerReceiver m_netPlyReceiver;
	NetGameEventReceiver m_netEventReceiver;
};