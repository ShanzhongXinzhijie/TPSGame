#pragma once
#include <unordered_set>
#include "GuiList.h"
#include "Network/Network.h"
#include "Network/NetGameEventCaster.h"

using namespace GameObj;

class Fade;

class ConfirmPlayers : public IGameObject{
public:
	ConfirmPlayers(Fade* fade, SuicideObj::CBGM* bgm = nullptr);
	~ConfirmPlayers();

	bool Start() override;
	void Update() override;

	void PostRender() override;

private:
	void ReloadRoomSettingFromProperies();

private:
	PerspectiveCamera camera;
	CDirectionLight m_dirlight;
	std::unordered_set<int> players;
	
	NetWorkManager* m_netWork = nullptr;
	NetGameEventCaster m_netEventCaster; bool m_isReady = false; bool m_allReady = false;

	//ゲーム設定
	bool m_isInitRoomSetting = false;
	float m_timeLimit = 160.0f;
	int m_citizenCnt = 401;
	
	GuiList list;
	CSprite m_sprite;
	CFont m_font;

	SuicideObj::CBGM* bgm;

	Fade* fade;
	int m_seed = 0, m_startTime = 0;//マスクラ用。Fadeに渡すラムダ関数用。
};

