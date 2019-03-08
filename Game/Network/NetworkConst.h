#pragma once

static const int NET_MAX_PLAYER = 20;

enum NetworkEventCode {
	enPlayerEventStart,
	enNormal = 1,
	enKillPlayer,
	enPlayerDeadOrAlive,
	enKenzoku,
	enPlayerEventEnd,

	enMasterClientEventStart,
	enMasterClientStartGame,
	enMasterClientEndGame,
	enMasterClientEventEnd,
};
enum PlayerStatusCode {
	enFrameCount = 0,
	enActionSender = 1,
	enFlyTimer = 10,
	enPosition = 11,
	enVelocity = 14,
	enBulletCnt = 20,
	enZombiePosAvg = 120,
	enZombiePosSync = 121,
	enZombiePos = 122,
};

//�v���C���[�v���p�e�B�̃L�[
static const ExitGames::Common::JString PLAYER_READY = L"r";

//���[���v���p�e�B�̃L�[
static const ExitGames::Common::JString TIME_LIMIT = L"t";
static const ExitGames::Common::JString CITIZEN_COUNT = L"c";

enum GameStartCode {
	enSeed,
	enSeverTime,
};