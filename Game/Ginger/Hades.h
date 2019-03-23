#pragma once

class CPlayer;
class GingerGene;

class Hades : public IGameObject
{
public:
	Hades(CPlayer* owner, GingerGene* gg);
	~Hades();

	bool Start()override;
	void Update()override;
	void PostRender()override;

private:
	//CFont m_Font;
	//int m_textCnt = 0;

	CPlayer* m_owner = nullptr;
	GingerGene* m_gingergene = nullptr;
};

