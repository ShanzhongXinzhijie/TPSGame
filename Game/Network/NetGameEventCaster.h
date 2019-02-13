#pragma once
#include"NetworkConst.h"

class NetGameEventCaster{
public:
	NetGameEventCaster();
	~NetGameEventCaster();

	void SendReady(bool ready);
	void SendRoomSetting_TimeLimit(float timelimit);
	void SendRoomSetting_CitizenCount(int count);

	void SendGameStart(int seed);
	void SendGameEnd(int zombieCnt[NET_MAX_PLAYER + 1]);

private:
};

