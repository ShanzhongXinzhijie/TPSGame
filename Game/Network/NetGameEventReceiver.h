#pragma once
#include"NetworkConst.h"

class NetGameEventReceiver{
public:
	NetGameEventReceiver();
	~NetGameEventReceiver();

	enum GameState {
		enGameStart,
		enGameEnd,
	};

	struct GameStartData {
		int m_seed = 0, m_startTime = 0;
	};
	struct GameEndData {
		int m_zombieCnt[NET_MAX_PLAYER+1] = {};
	};

	//イベント受信
	void EventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj);

	//データを取得
	const GameState& GetGameState()const { return m_gameState; }
	const GameStartData& GetGameStartData()const { return m_gameStartData; }
	const GameEndData& GetGameEndData()const { return m_gameEndData; }	

private:
	GameState m_gameState = enGameEnd;
	GameStartData	m_gameStartData;
	GameEndData		m_gameEndData;
};

