#pragma once

static const int NET_MAX_PLAYER = 20;

enum NetworkEventCode {
	enActionSender = 1,
	enPosition,
	enIsDead,
	//enJoinLeaveEvent,
};