#pragma once
#include <unordered_map>

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
	std::unordered_map<int, CVector4> players;
	bool startButton[3] = {};
	
	CSprite m_sprite;

};

