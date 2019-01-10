#pragma once
#include <unordered_set>
#include "GuiList.h"
#include "Network/Network.h"

using namespace GameObj;

class ConfirmPlayers : public IGameObject{
public:
	ConfirmPlayers();
	~ConfirmPlayers();

	bool Start() override;
	void Update() override;

	void PostRender() override;

private:

	PerspectiveCamera camera;
	CDirectionLight m_dirlight;
	std::unordered_set<int> players;
	
	NetWorkManager* m_netWork = nullptr;

	//NetJoinLeaveCaster m_caster;
	
	GuiList list;
	CSprite m_sprite;
};

