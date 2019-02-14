#pragma once

class FlyWalker;

class Wing : public IGameObject{
public:
	Wing(FlyWalker& player);
	~Wing();

	bool Start() override;
	void Update() override;

	void closeDelete();

private:
	FlyWalker& player;
	GameObj::CSkinModelRender m_model;
	AnimationClip anim[1];
	CVector3 size = {0,0,0};

	static const CVector3 hight;
	static constexpr float openSpeed = 1.0f / 0.5f;

	bool closing = false;
};

