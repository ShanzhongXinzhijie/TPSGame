#pragma once
#include <unordered_set>

class CPlayer;

using namespace GameObj;

class ConfirmPlayers : public IGameObject{
public:
	ConfirmPlayers();
	~ConfirmPlayers();

	bool Start() override;
	void Update() override;

private:
	PerspectiveCamera camera;
	CDirectionLight m_dirlight;
	CSkinModelRender myModel;
	CSkinModelRender* pModels[3];
	int modelCount = 0;
	std::unordered_set<int> players;
	
};

