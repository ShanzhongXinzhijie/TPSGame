#pragma once
class GingerGene
{
public:
	GingerGene();
	~GingerGene();

	void Register(IGameObject* go);
	void Release();

	void Create(float gameTimelimit);

private:
	std::list<IGameObject*> m_deleteList;
};

