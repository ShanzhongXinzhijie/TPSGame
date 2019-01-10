#pragma once
class Result : public IGameObject{
public:
	Result(unsigned int red, unsigned int blue);
	~Result();

	bool Start() override;
	void Update() override;
	void PostRender() override;

private:
	unsigned int red;
	unsigned int blue;

	GameObj::OrthoCamera camera;
	CFont m_font;
	CSprite m_sprite;
};

