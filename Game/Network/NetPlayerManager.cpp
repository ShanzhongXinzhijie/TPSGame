#include "stdafx.h"
#include "NetPlayerManager.h"
#include "Network.h"
#include "../Include/Game.h"

NetPlayerManager::NetPlayerManager() : list(L"Resource/spriteData/waku.dds")
{
	list.depth = 0.0f;//最前面に表示
}


NetPlayerManager::~NetPlayerManager()
{
	//プレイヤー削除
	for (const std::pair<int, NetPlayerCaster*>& ppp : m_playerCastersMap) {
		m_netReceiver->RemovePlayer(ppp.first);
		delete ppp.second;
		m_game->removePlayer(ppp.first);
	}

	//関数解除
	GetPhoton()->SetJoinEventAction(
		nullptr
	);
	GetPhoton()->SetLeaveEventAction(
		nullptr
	);
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

void NetPlayerManager::Init(Game* pGame, CitizenGene* citizenGene) {
	m_game = pGame;
	m_citizenGene = citizenGene;
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
		m_playerCastersMap[playerNr] = new NetPlayerCaster(m_game->getPlayer(playerNr), m_citizenGene);
		m_netReceiver->SetPlayer(m_game->getPlayer(playerNr), m_playerCastersMap[playerNr]);
	}
}

void NetPlayerManager::Update() {
	if (Pad(0).GetDown(enButtonBack)) {
		m_isView = !m_isView;
	}
}
void NetPlayerManager::PostRender() {

	if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) {
		m_font.Draw(L"Disconnect\n\"サイキドウ\"シテクダサイ...", { 0.5f - 0.001f,0.5f - 0.001f }, { 1.0f, 1.0f ,1.0f,1.0f }, CVector2::One(), { 0.5f,0.5f }, 0.0f, DirectX::SpriteEffects_None, 0.0f);
		m_font.Draw(L"Disconnect\n\"サイキドウ\"シテクダサイ...", { 0.5f + 0.001f,0.5f + 0.001f }, { 0.0f, 0.0f ,0.0f,1.0f }, CVector2::One(), { 0.5f,0.5f }, 0.0f, DirectX::SpriteEffects_None, 0.0f);
		m_font.Draw(L"Disconnect\n\"サイキドウ\"シテクダサイ...", { 0.5f,0.5f }, { 1.0f, 0.0f ,0.0f,1.0f }, CVector2::One(), {0.5f,0.5f}, 0.0f, DirectX::SpriteEffects_None, 0.0f);
	}

	if (!m_isView) { return; }

	//Ping表示
	if (GetPhoton()->GetState() == PhotonNetworkLogic::JOINED) {
		list.color = CVector4::White();
		wchar_t str[128];
		swprintf_s(str, L"Ping: %dms  Seed:%d", GetPhoton()->GetPing_ms(), m_viewSeed);
		list.values.emplace_back(str);
	}
	else {
		list.color = {1.0f,0.0f,0.0f,1.0f};
		list.values.emplace_back(L"Disconnect");
	}

	//ルーム内のプレイヤーを表示
	const ExitGames::Common::JVector<ExitGames::LoadBalancing::Player*>& players = GetPhoton()->GetPlayers();
	for (unsigned int i = 0; i < players.getSize(); i++) {
		list.values.emplace_back(std::to_wstring(players[i]->getNumber()));
		list.values.back() += L" :";
		list.values.back() += players[i]->getName().cstr();
		if (players[i]->getNumber() == GetPhoton()->GetLocalPlayerNumber()) {
			list.values.back() += L" <<YOU";
		}
		if (players[i]->getIsMasterClient()) {
			list.values.back() += L" <<Master";
		}
	}

	list.fontscale = { 0.62f,0.62f };
	list.Draw();

	list.values.clear();
}