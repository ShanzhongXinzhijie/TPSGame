#pragma once

static constexpr int NET_MAX_PLAYER = 6;// 10;
static constexpr int NET_SEND_RATE[10] = {4,4,4,4,5,8,12,16,20,25};

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
	enPosition = 12,
	enVelocity = 15,
	enBulletCnt = 21,
	enZombiePosAvg = 120,
	enZombiePosSync = 121,
	enZombieMover = 122,
	enZombiePos = 123,
};
enum ReliableDataCode {
	//enDestroyGinger,
	enGetGodpower = 2,
	enUseGodPower = 4,
	enIndraArrow,
	enUseOdin = enIndraArrow + 3,
	enSummonWosiris = enUseOdin + 3,
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