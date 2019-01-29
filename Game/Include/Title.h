#pragma once
using namespace GameObj;

class Fade;

class Title : public IGameObject{
public:
	Title(Fade* fade);
	~Title();
	
	bool Start() override;
	void Update() override;
	void PostRender() override;

private:
	OrthoCamera camera;
	CFont m_font;
	CSprite m_sprite;

	Fade* fade;
};

