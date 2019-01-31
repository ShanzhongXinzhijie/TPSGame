#include "stdafx.h"
#include "ConfirmPlayers.h"
#include "CPlayer.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"

#include "Network/Network.h"

ConfirmPlayers::ConfirmPlayers(Fade* fade) : list(L"Resource/spriteData/waku.dds") {
	this->fade = fade;
	fade->fadeOut();
	
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
	for (int i = 0; i < 8; i++) {
		if (randam < min + 1.0f / 8.0f) {
			switch (i)
			{
			case 0:
				wstr = L"エルフ";
				break;
			case 1:
				wstr = L"ロイヤル";
				break;
			case 2:
				wstr = L"ウィッチ";
				break;
			case 3:
				wstr = L"ドラゴン";
				break;
			case 4:
				wstr = L"ネクロ";
				break;
			case 5:
				wstr = L"ヴァンパイア";
				break;
			case 6:
				wstr = L"ビショップ";
				break;
			case 7:
				wstr = L"ネメシス";
				break;
			default:
				break;
			}
			wstr += L"中村";
			break;
		}
		min += 1.0f / 8.0f;
	}
	//サーバーに接続(その後勝手にルームに入る)
	m_netWork->Connect(wstr.c_str());
#endif

	return true;
}

void ConfirmPlayers::Update() {
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
#ifdef SpritScreen
		GetCameraList().clear();
#endif
		bool isReady = true;
#ifndef SpritScreen
		//ルームに入るのを待つ
		if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) { isReady = false; }
#endif
		if (isReady) {
			//ゲーム開始
			fade->fadeIn([&]() {
				new Game(fade,m_timeLimit,m_citizenCnt);
				delete this;
			});
		}
	} else if (Pad(0).GetDown(enButtonBack)) {
#ifndef SpritScreen
		//サーバーから切断
		m_netWork->Disconnect();
#endif
		//タイトルに戻る
		fade->fadeIn([&]() {
			new Title(fade);
			delete this;
		});
	}

	//ゲーム設定
	if (Pad(0).GetDown(enButtonUp)) {
		m_timeLimit += 10.0f;
	}
	if (Pad(0).GetDown(enButtonDown)) {
		m_timeLimit -= 10.0f; m_timeLimit = max(1.0f, m_timeLimit);
	}
	if (Pad(0).GetDown(enButtonLeft)) {
		m_citizenCnt -= 50; m_citizenCnt = max(1, m_citizenCnt);
	}
	if (Pad(0).GetDown(enButtonRight)) {
		m_citizenCnt += 25;
	}
}

void ConfirmPlayers::PostRender() {
	m_sprite.Draw({ 0.5f, 0.5f }, CVector2::One(), { 0.5f, 0.5f });

	if (GetPhoton()->GetState() == PhotonNetworkLogic::JOINED) {
		//Ping表示
		wchar_t str[64];
		swprintf_s(str, L"Ping: %dms", GetPhoton()->GetPing_ms());
		list.values.emplace_back(str);

		//ルーム内のプレイヤーを表示
		const ExitGames::Common::JVector<ExitGames::LoadBalancing::Player*>& players = GetPhoton()->GetPlayers();
		for (unsigned int i = 0; i < players.getSize(); i++){
			list.values.emplace_back(std::to_wstring(players[i]->getNumber()));
			list.values.back() += L" :";
			list.values.back() += players[i]->getName().cstr();
			if (players[i]->getNumber() == GetPhoton()->GetLocalPlayerNumber()) {
				list.values.back() += L" <<自分";
			}
			if (players[i]->getIsMasterClient()) {
				list.values.back() += L" <<ﾏｽｸﾗ";
			}
		}
	}
	else if (GetPhoton()->GetState() == PhotonNetworkLogic::DISCONNECTED) {
		list.values.emplace_back(L"切断");
	}
	else {
#ifdef SpritScreen
		list.values.emplace_back(L"オフラインモード");
#else
		list.values.emplace_back(L"接続中...");
#endif
	}

	list.Draw();

	list.values.clear();

	//ゲーム設定表示
	{
		wchar_t str[128];
		swprintf_s(str, L"制限時間:%.1f\n人口:%d", m_timeLimit, m_citizenCnt);
		m_font.Draw(str, { 1.0f,0.0f }, CVector4::White(), CVector2::One(), { 1.0f,0.0f });
	}
}
