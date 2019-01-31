#pragma once
#include <unordered_set>
#include "GuiList.h"
#include "Network/Network.h"

using namespace GameObj;

class Fade;

class ConfirmPlayers : public IGameObject{
public:
	ConfirmPlayers(Fade* fade);
	~ConfirmPlayers();

	bool Start() override;
	void Update() override;

	void PostRender() override;

private:

	PerspectiveCamera camera;
	CDirectionLight m_dirlight;
	std::unordered_set<int> players;
	
	NetWorkManager* m_netWork = nullptr;

	//ÉQÅ[ÉÄê›íË
	float m_timeLimit = 160.0f;
	int m_citizenCnt = 1;
	
	GuiList list;
	CSprite m_sprite;
	CFont m_font;

	Fade* fade;
};

