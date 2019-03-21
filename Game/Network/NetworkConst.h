#pragma once

static const int NET_MAX_PLAYER = 20;

enum NetworkEventCode {
	enPlayerEventStart,
	enNormal = 1,
	enKillPlayer,
	enPlayerDeadOrAlive,
	enReliable,
	enDestroyGinger,
	enPlayerEventEnd,

	enMasterClientEventStart,
	enMasterClientStartGame,
	enMasterClientEndGame,
	enMasterClientEventEnd,
};
enum PlayerStatusCode {
	enFrameCount = 0,
	enActionSender = 1,
	enLockOn = 8,
	enActiveWepon = 9,
	enFlyTimer = 10,
	enPosition = 11,
	enVelocity = 14,
	enBulletCnt = 20,
	enZombiePosAvg = 120,
	enZombiePosSync = 121,
	enZombieMover = 122,
	enZombiePos = 123,
};
enum ReliableDataCode {
	//enDestroyGinger,
	enGetGodpower = 2,
	enUseGodPower = 4,
	enSummonWosiris,
	enGetControlWosiris = enSummonWosiris + 4,
	enKenzoku = enGetControlWosiris + 4,
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