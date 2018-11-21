#pragma once
class BatBullet : public IGameObject{
public:
	BatBullet(CVector3 position, CVector3 direction);
	~BatBullet();
	bool Start() override;
	void Update() override;

private:
	GameObj::CSkinModelRender m_model;
	CVector3 m_pos;
	CVector3 m_dir;
	float lifeTime = 5;
};

