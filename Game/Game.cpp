#include "stdafx.h"
#include "Game.h"
#include "Ground.h"
#include "CircleWalk.h"
#include "sikakukuidou.h"
#include "ziguzagu.h"
#include "ittarikitari.h"
#include "Result.h"
#include "Fade.h"
#include "GameWaiter.h"
#include "Ginger/Ginger.h"

Game* Game::static_game = nullptr;

Game::Game(Fade* fade, float timeLimit, int citizenCnt, int seed, int startTime_ms) : citizenGene(this), timer(timeLimit){
	//市民とプレイヤーの発生位置の設定
	level.Init(L"Resource/Level/level.tkl", [&](LevelObjectData& objData)->bool {
		if (objData.EqualObjectName(L"player700")) {
			playerGene.addSpawner(objData.position, 780.0f);
		} else if (objData.EqualObjectName(L"citizen600")) {
			citizenGene.addSpawner(objData.position, 600.0f);
		} else if (objData.EqualObjectName(L"citizen1800")) {
			citizenGene.addSpawner(objData.position, 1800.0f);
		}
		return true;
	});
	//舞台の生成
	ground = new Ground(CVector3::Zero());

	bgm = NewGO<SuicideObj::CBGM>(L"Resource/sound/BGM_battle.wav");
	bgm->SetVolume(0.4f);
	bgm->Play(false, true);

#ifdef SpritScreen
	createPlayer(true, 0);
	createPlayer(true, 1);
#else
	//プレイヤーマネージャーの初期化(プレイヤーの作成)
	m_netPlayerManager.Init(this, &citizenGene);
	m_netPlayerManager.SetViewSeed(seed);
#endif

	//乱数同期
	srand(seed);
	//市民作成
	citizenGene.createCitizen(citizenCnt);

#ifndef SpritScreen
	m_netWork = FindGO<NetWorkManager>(L"NetWorkManager");
	m_netWork->GetNetPlayerReceiver().SetCitizenGene(&citizenGene);
	m_netWork->GetNetPlayerReceiver().SetGingerGene(&gingerGene);
#endif

	//神社作成
	gingerGene.Create(timeLimit, &m_netWork->GetNetPlayerReceiver());

#ifdef SpritScreen
	karicamera.SetPos({ 600,600,600 });
	karicamera.SetTarget({0, 0, 0});
	karicamera.SetFar(3000);
	GetCameraList().push_back(&karicamera);
	GetCameraList().push_back(&karicamera);
#endif

#ifndef SpritScreen
	if (GetPhoton()->GetPlayers().getSize() > 1) {
		//ゲーム開始タイマー設定
		int t = GetPhoton()->GetSeverTime_ms();
		if (t < 0 && startTime_ms >= 0) { startTime_ms = INT_MIN; }
		m_waitGameStartTimer_sec = CMath::Clamp(15.0f - (t - startTime_ms)*0.001f, 0.0f, 15.0f);
		//ゲームをウェイト状態にする
		if (m_waitGameStartTimer_sec > 0.0f) { GameWaiter::SetIsWait(true); }
	}
#endif

	this->fade = fade;
	fade->fadeOut();
	//次のフレームの可変フレーム無効
	//GetEngine().UnableVariableFramerateOnce();
}

Game::~Game() {
	bgm->Stop();
	delete ground;

#ifndef SpritScreen
	m_netWork->GetNetPlayerReceiver().SetCitizenGene(nullptr);
	m_netWork->GetNetPlayerReceiver().SetGingerGene(nullptr);
#endif
	GameWaiter::SetIsWait(false);
	static_game = nullptr;
}

void Game::Update() {
	//開始前待機時間
	if (m_waitGameStartTimer_sec > 0.0f) {
		m_waitGameStartTimer_sec -= GetDeltaTimeSec();
		if (m_waitGameStartTimer_sec <= 0.0f) {
			m_waitGameStartTimer_sec = 0.0f;
			//ウェイト状態の解除
			GameWaiter::SetIsWait(false);
		}
		else {
			return;
		}
	}

	//制限時間の減少
	if (timer > fadeInDelay) {
		timer -= GetDeltaTimeSec();
	}

	//タイムアップ
	if (timer < 0 && !GameWaiter::GetIsWait()) {
		GameWaiter::SetIsWait(true);
		NewGO<SuicideObj::CSE>(L"Resource/sound/SE_timeUp.wav")->Play(false);
	}

	//ゲームの終了処理
	if (timer <= fadeInDelay && !gameIsEnd) {
		timer = fadeInDelay;
		gameIsEnd = true;
#ifdef SpritScreen
		fade->fadeIn([&]() {
			new Result(playerGene, fade);
			delete this; return;
		});
#else
		if (GetPhoton()->GetIsMasterClient()) {
			//マスタークライアント
			
			//ゾンビの数セット
			int zombieCnt[NET_MAX_PLAYER + 1];
			std::vector<Team*> teamPtrs = playerGene.getTeams();
			for (int i = 0; i < min(teamPtrs.size(), NET_MAX_PLAYER + 1); i++) {
				zombieCnt[i] = teamPtrs[i]->getZombieCount();
			}
			//終了命令送信
			m_netEventCaster.SendGameEnd(zombieCnt);
			GameWaiter::SetIsWait(true);

			//リザルトへ
			fade->fadeIn([&]() {
				new Result(playerGene, fade);
				delete this; return;
			});
		}
#endif
	}	

#ifndef SpritScreen
	if (!GetPhoton()->GetIsMasterClient()) {
		//通常クライアント

		//終了命令来てたら終了
		if (m_netWork->GetNetEventReceiver().GetGameState() == NetGameEventReceiver::enGameEnd) {

			//ゾンビの数同期
			std::vector<Team*> teamPtrs = playerGene.getTeams();
			for (int i = 0; i < min(teamPtrs.size(), NET_MAX_PLAYER + 1); i++) {
				teamPtrs[i]->setZombie(m_netWork->GetNetEventReceiver().GetGameEndData().m_zombieCnt[i]);
			}

			GameWaiter::SetIsWait(true);

			//リザルトへ
			fade->fadeIn([&]() {
				new Result(playerGene, fade);
				delete this; return;
			});
		}
	}
#endif

}

void Game::PostRender() {
	//開始前待機時間の描画
	if (m_waitGameStartTimer_sec > 0.0f) {
		wchar_t countDisp[24];
		swprintf_s(countDisp, L"Ready... %.1f sec", m_waitGameStartTimer_sec);
		font.Draw(countDisp, { 0.503f, 0.703f }, {0,0,0,1}, CVector2::One(), { 0.5f,0.5f }, 0.0f, DirectX::SpriteEffects_None, 0.0f);
		font.Draw(countDisp, { 0.5f, 0.7f }, CVector4::White(), CVector2::One(), { 0.5f,0.5f },0.0f,DirectX::SpriteEffects_None,0.0f);
		return;
	}

	wchar_t countDisp[20];
	float l_timer = timer;
	if (l_timer < 0) {
		l_timer = 0;
	}//制限時間の描画
	swprintf_s(countDisp, L"Time: %.1f sec", l_timer);
	font.Draw(countDisp, { 0.013f, 0.013f }, { 0,0,0,1 }, {1.2f,1.2f});
	font.Draw(countDisp, { 0.01f, 0.01f }, { 1,1,1,1 }, { 1.2f,1.2f });

	if (timer < 0) {//ゲーム終了後、TimeUpと表示する
		float scale = timer / -0.5f;
		if (scale > 1.0f) {
			scale = 1.0f;
		}
		scale *= 5;
		const wchar_t* timeUp = L"TimeUp!!";
		font.Draw(timeUp, { 0.503f, 0.503f }, { 0,0,0,1 }, { scale,scale }, { 0.55f,0.6f },
				  0.0f, DirectX::SpriteEffects_None, 0.4f);
		font.Draw(timeUp, { 0.5f, 0.5f }, { 0.2f,0.6f,1,1 }, { scale,scale }, {0.55f,0.6f},
0.0f, DirectX::SpriteEffects_None, 0.4f);
	}
}

void Game::createPlayer(bool isMe, int playerNum) {
	playerGene.createPlayer(isMe, playerNum);
}

void Game::removePlayer(int playerNum) {
	playerGene.removePlayer(playerNum);
}

