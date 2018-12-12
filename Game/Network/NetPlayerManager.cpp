#include "stdafx.h"
#include "NetPlayerManager.h"
#include "Network.h"
#include "../Include/Game.h"

NetPlayerManager::NetPlayerManager()
{
}


NetPlayerManager::~NetPlayerManager()
{
	for (const std::pair<int, NetPlayerCaster*>& ppp : m_playerCastersMap) {
		m_netReceiver->RemovePlayer(ppp.first);
		delete ppp.second;
		m_game->removePlayer(ppp.first);
	}
}


void NetPlayerManager::JoinEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) {
	//プレイヤー作成
	CreatePlayer(playerNr);
}
void NetPlayerManager::LeaveEventAction(int playerNr, bool isInactive) {
	//プレイヤー削除
	m_netReceiver->RemovePlayer(playerNr);	
	delete m_playerCastersMap[playerNr];
	m_playerCastersMap.erase(playerNr);
	m_game->removePlayer(playerNr);
}

void NetPlayerManager::Init(Game* pGame) {
	m_game = pGame;
	m_netReceiver = &FindGO<NetWorkManager>(L"NetWorkManager")->GetNetPlayerReceiver();

	const ExitGames::Common::JVector<ExitGames::LoadBalancing::Player*>& players = GetPhoton()->GetPlayers();

	//ルームに居る人数分、プレイヤーを作成する
	for (unsigned int i = 0; i < players.getSize(); i++) {
		//プレイヤー作成
		CreatePlayer(players[i]->getNumber());
	}

	//プレイヤーの入退出時の処理を設定
	GetPhoton()->SetJoinEventAction(
		[&](int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) {
			JoinEventAction(playerNr, playernrs, player);
		}
	);
	GetPhoton()->SetLeaveEventAction(
		[&](int playerNr, bool isInactive) {
			LeaveEventAction(playerNr, isInactive);
		}
	);
}

void NetPlayerManager::CreatePlayer(int playerNr) {
	//重複を許さない
	if (m_playerCastersMap.count(playerNr) == 0) {
		//自分自身か？
		bool isMe = false;
		if (playerNr == GetPhoton()->GetLocalPlayerNumber()) {
			isMe = true;
		}
		//プレイヤー作成
		m_game->createPlayer(isMe, playerNr);
		m_playerCastersMap[playerNr] = new NetPlayerCaster(m_game->getPlayer(playerNr));
		m_netReceiver->SetPlayer(m_game->getPlayer(playerNr), m_playerCastersMap[playerNr]);
	}
}

void NetPlayerManager::PostRender() {
	//接続状態の表示
	if (GetPhoton()->GetState() == PhotonNetworkLogic::JOINED) {
		wchar_t str[256];
		swprintf_s(str, L"接続\nPing: %dms", GetPhoton()->GetPing_ms());
		m_font.Draw(str, { 0.0f,0.2f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	}
	else {
		m_font.Draw(L"切断", { 0.0f,0.2f }, { 1.0f, 0.0f, 0.0f, 1.0f });
	}
}