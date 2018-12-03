#pragma once
using namespace GameObj;

class Title : public IGameObject{
public:
	Title();
	~Title();
	
	bool Start() override;
	void Update() override;
	void PostRender() override;

private:
	OrthoCamera camera;
	CFont m_font;
	CSprite m_sprite;
};

