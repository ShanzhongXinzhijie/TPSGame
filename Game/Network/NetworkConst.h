#pragma once

static const int NET_MAX_PLAYER = 20;

enum NetworkEventCode {
	enPlayerEventStart,

	enPlayerStatus = 1,
	enKillPlayer,
	enPlayerDeadOrAlive,

	enPlayerEventNum,
};
enum PlayerStatusCode {
	enFrameCount = 0,
	enActionSender = 1,
	enPosition = 11,
};