#pragma once
using namespace GameObj;

class Title : public IGameObject{
public:
	Title();
	~Title();
	
	bool Start() override;
	void Update() override;

private:
	CSkinModelRender m_render;
	OrthoCamera camera;
	CDirectionLight m_dirlight;
};

