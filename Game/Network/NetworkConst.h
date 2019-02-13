#pragma once

static const int NET_MAX_PLAYER = 20;

enum NetworkEventCode {
	enPlayerEventStart,
	enPlayerStatus = 1,
	enKillPlayer,
	enPlayerDeadOrAlive,
	enPlayerEventEnd,

	enMasterClientEventStart,
	enMasterClientStartGame,
	enMasterClientEndGame,
	enMasterClientEventEnd,
};
enum PlayerStatusCode {
	enFrameCount = 0,
	enActionSender = 1,
	enPosition = 11,
};

//プレイヤープロパティのキー
static const ExitGames::Common::JString PLAYER_READY = L"r";

//ルームプロパティのキー
static const ExitGames::Common::JString TIME_LIMIT = L"t";
static const ExitGames::Common::JString CITIZEN_COUNT = L"c";

enum GameStartCode {
	enSeed,
	enSeverTime,
};