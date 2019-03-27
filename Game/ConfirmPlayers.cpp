#include "stdafx.h"
#include "ConfirmPlayers.h"
#include "CPlayer.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"
#include "DeckType.h"
#include "Network/Network.h"

ConfirmPlayers::ConfirmPlayers(Fade* fade, SuicideObj::CBGM* bgm)
	: list(L"Resource/spriteData/waku.dds") , bgm(bgm){
	this->fade = fade;
	fade->fadeOut();
	
	if (bgm == nullptr) {
		this->bgm = NewGO<SuicideObj::CBGM>(L"Resource/sound/BGM_title.wav");
		this->bgm->Play(false, true);
	}

	camera.SetPos({ 0, 50, 200 });
	camera.SetTarget({ 0, 50, 0 });
	camera.UpdateMatrix();

#ifdef SpritScreen
	GetCameraList().clear();
	GetCameraList().push_back(&camera);
	GetCameraList().push_back(&camera);
#else
	SetMainCamera(&camera);
#endif
}


ConfirmPlayers::~ConfirmPlayers() {
}

bool ConfirmPlayers::Start() {

	m_dirlight.SetColor({ 1,1,1 });
	m_dirlight.SetDirection({ 0,0,1 });

	m_sprite.Init(L"Resource/spriteData/Title.dds");

	m_netWork = FindGO<NetWorkManager>(L"NetWorkManager");

#ifndef SpritScreen
	//プレイヤーネーム決める
	std::wstring wstr;
	float randam = CMath::RandomZeroToOne();
	float min = 0.0f;

	//名前作成
	int r = 0;
	r = CMath::RandomInt() % DECK_TYPE_MAX;
	wstr += DECK_TYPE[r];
	wstr += L" ";
	r = CMath::RandomInt() % CHARA_TYPE_MAX;
	wstr += CHARA_TYPE[r];
	wstr += L" ";
	wstr += L"NAKAMURA";

	//サーバーに接続(その後勝手にルームに入る)
	m_netWork->Connect(wstr.c_str());
#endif

	return true;
}

void ConfirmPlayers::Update() {

	if (m_isGameStart) {
		return;
	}

#ifndef SpritScreen
	//エラー出たら切断
	if (m_netWork->GetIsError()) {
		m_netWork->Disconnect();
	}
#endif

#ifdef SpritScreen
	for (int num = 1; num < 4; num++) {

		if (Pad(num).GetDown(enButtonStart)) {
			if (players.count(num) == 0) {
				players.insert(num);
			} else {
				players.erase(num);
			}
		}
	}
#endif

	if (Pad(0).GetDown(enButtonStart)) {
		bool isJoin = true;
#ifdef SpritScreen
		GetCameraList().clear();
#else
		//ルームに入るのを待つ
		if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) { isJoin = false; }
#endif
		if (isJoin && fade->isIdel()) {
#ifdef SpritScreen
			int seed = CMath::RandomInt();
			//ゲーム開始
			NewGO<SuicideObj::CSE>(L"Resource/sound/SE_select.wav")->Play();
			m_isGameStart = true;
			fade->fadeIn([&]() {
				bgm->Stop();
				Game::createGame(fade,m_timeLimit,m_citizenCnt, seed, -1);
				delete this; return;
			});
#else
			//Ready!
			m_netEventCaster.SendReady(m_isReady ? false : true);
#endif
		}
	} 
	else if (Pad(0).GetDown(enButtonBack)) {//タイトルに戻る
#ifndef SpritScreen
		//サーバーから切断
		m_netWork->Disconnect();
#endif
		//タイトルに戻る
		fade->fadeIn([&]() {
			Title* T = new Title(fade, bgm); T->SetName(L"TitleClass");
			delete this; return;
		});
	}

#ifndef SpritScreen
	if (GetPhoton()->GetState() == PhotonNetworkLogic::JOINED) {
	if (GetPhoton()->GetIsMasterClient()) {
		//マスタークライアント

		bool start = false;
		//強制的に開始
		if (Pad(0).GetButton(enButtonA) && Pad(0).GetButton(enButtonStart)) {
			start = true;
		}
		//全員Readyで開始
		if (m_allReady) {
			start = true;
		}
		if (start) {
			//Ready初期化しとく
			m_isReady = false;
			m_netEventCaster.SendReady(false);

			//ゲーム設定とゲーム開始送信
			m_netEventCaster.SendRoomSetting_TimeLimit(m_timeLimit);
			m_netEventCaster.SendRoomSetting_CitizenCount(m_citizenCnt);
			int seed = CMath::RandomInt();
			m_netEventCaster.SendGameStart(seed);

			//保存
			m_seed = seed;
			m_startTime = GetPhoton()->GetSeverTime_ms();

			//ゲーム開始
			m_isGameStart = true;
			fade->fadeIn([&]() {
				bgm->Stop();
				Game::createGame(fade, m_timeLimit, m_citizenCnt, m_seed, m_startTime);
				delete this; return;
			});
		}
	}
	else if(m_netWork->GetNetEventReceiver().GetGameState() == NetGameEventReceiver::enGameStart){
		//通常クライアント

		//Ready初期化しとく
		m_isReady = false;
		m_netEventCaster.SendReady(false);

		const ExitGames::Common::Hashtable& properties = GetPhoton()->GetRoomProperty();
		m_timeLimit = ((ExitGames::Common::ValueObject<float>*)(properties.getValue(TIME_LIMIT)))->getDataCopy();
		m_citizenCnt = ((ExitGames::Common::ValueObject<int>*)(properties.getValue(CITIZEN_COUNT)))->getDataCopy();

		//ゲーム開始
		m_isGameStart = true;
		fade->fadeIn([&]() {
			bgm->Stop();
			Game::createGame(fade, m_timeLimit, m_citizenCnt, m_netWork->GetNetEventReceiver().GetGameStartData().m_seed, m_netWork->GetNetEventReceiver().GetGameStartData().m_startTime);
			delete this; return;
		});
	}
	}
#endif

	bool isCanChangeSetting = true;
#ifndef SpritScreen
	if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED || !GetPhoton()->GetIsMasterClient()) {
		isCanChangeSetting = false; 
	}
#endif
	if (isCanChangeSetting) {
		//ゲーム設定をルームプロパティから初期化
		if (!m_isInitRoomSetting) {
			ReloadRoomSettingFromProperies();
			m_isInitRoomSetting = true;
		}
		//ゲーム設定
		if (Pad(0).GetDown(enButtonUp)) {
			m_timeLimit += 10.0f;
			m_netEventCaster.SendRoomSetting_TimeLimit(m_timeLimit);
		}
		if (Pad(0).GetDown(enButtonDown)) {
			m_timeLimit -= 10.0f; m_timeLimit = max(1.0f, m_timeLimit);
			m_netEventCaster.SendRoomSetting_TimeLimit(m_timeLimit);
		}
		if (Pad(0).GetDown(enButtonLeft)) {
			m_citizenCnt -= 50; m_citizenCnt = max(1, m_citizenCnt);
			m_netEventCaster.SendRoomSetting_CitizenCount(m_citizenCnt);
		}
		if (Pad(0).GetDown(enButtonRight)) {
			m_citizenCnt += 25;
			m_netEventCaster.SendRoomSetting_CitizenCount(m_citizenCnt);
		}
	}
}

void ConfirmPlayers::PostRender() {
	m_sprite.Draw({ 0.5f, 0.5f }, CVector2::One(), { 0.5f, 0.5f });

	if (GetPhoton()->GetState() == PhotonNetworkLogic::JOINED) {
		//Ping表示
		wchar_t str[64];
		swprintf_s(str, L"Ping: %dms", GetPhoton()->GetPing_ms());
		list.values.emplace_back(str);

		//Readyをカウント
		int readyCnt = 0;

		//ルーム内のプレイヤーを表示
		const ExitGames::Common::JVector<ExitGames::LoadBalancing::Player*>& players = GetPhoton()->GetPlayers();
		for (unsigned int i = 0; i < players.getSize(); i++){
			list.values.emplace_back(std::to_wstring(players[i]->getNumber()));
			list.values.back() += L" :";
			list.values.back() += players[i]->getName().cstr();

			bool isMine = false;
			if (players[i]->getNumber() == GetPhoton()->GetLocalPlayerNumber()) {
				isMine = true;
				list.values.back() += L" <<It's me";
			}
			if (players[i]->getIsMasterClient()) {
				list.values.back() += L" <<Master";
			}

			const ExitGames::Common::Hashtable& eventContent = players[i]->getCustomProperties();
			if (eventContent.getValue(PLAYER_READY)
				&& ((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue(PLAYER_READY)))->getDataCopy()
			) {
				list.values.back() += L" <<Ready!";
				if (isMine) { m_isReady = true; }
				readyCnt++;
			}
			else {
				if (isMine) { m_isReady = false; }
			}
		}

		if (readyCnt == players.getSize()) {
			//全員Ready
			m_allReady = true;
		}
	}
	else if (GetPhoton()->GetState() == PhotonNetworkLogic::DISCONNECTED) {
		list.values.emplace_back(L"disconnect\n");
		list.values.back() += m_netWork->GetErrorMessage();
		list.values.back() += L"\n\n(BACK)ボタンでモドル";
	}
	else {
#ifdef SpritScreen
		list.values.emplace_back(L"オフラインモード");
#else
		list.values.emplace_back(L"Connecting...\n");
		list.values.back() += m_netWork->GetErrorMessage();
#endif
	}

	list.fontscale = { 0.62f,0.62f };
	list.Draw();

	list.values.clear();

	//ゲーム設定表示
	{
#ifndef SpritScreen
		//通常クライアントは情報更新
		if (GetPhoton()->GetState() == PhotonNetworkLogic::JOINED && !GetPhoton()->GetIsMasterClient()) {
			ReloadRoomSettingFromProperies();
		}
#endif
		wchar_t str[128];
		swprintf_s(str, L"Time:%.1f\nCitizen:%d", m_timeLimit, m_citizenCnt);
		m_font.Draw(str, { 0.86f,0.0f }, CVector4::White(), CVector2::One(), { 0.0f,0.0f });
		m_font.Draw(L"< backボタンでタイトルにもどる",
					{ 0.01f,0.0f }, CVector4::White(), {0.7f,0.7f}, { 0.0f,0.0f });
		if (!m_isReady) {
			m_font.Draw(L"startボタンでじゅんびかんりょう",
						{ 0.5f,0.95f }, CVector4::White(), { 0.7f,0.7f }, { 0.5f,0.0f });
		} else {
			m_font.Draw(L"ぜんいんそろったらかいし",
						{ 0.5f,0.95f }, CVector4::White(), { 0.7f,0.7f }, { 0.5f,0.0f });
		}
	}
}

void ConfirmPlayers::ReloadRoomSettingFromProperies() {
	const ExitGames::Common::Hashtable& properties = GetPhoton()->GetRoomProperty();
	if (properties.getValue(TIME_LIMIT)) {
		m_timeLimit = ((ExitGames::Common::ValueObject<float>*)(properties.getValue(TIME_LIMIT)))->getDataCopy();
	}
	if (properties.getValue(CITIZEN_COUNT)) {
		m_citizenCnt = ((ExitGames::Common::ValueObject<int>*)(properties.getValue(CITIZEN_COUNT)))->getDataCopy();
	}
}