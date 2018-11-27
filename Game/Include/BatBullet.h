#pragma once
class BatBullet : public IGameObject{
public:
	BatBullet(int playerNum, CVector3 position, CVector3 direction);
	~BatBullet();
	bool Start() override;
	void Update() override;

private:
	CVector3 getHitVec() const;

	GameObj::CSkinModelRender m_model;
	CVector3 m_pos;
	CVector3 m_dir;
	float lifeTime = 5;
	const int shotPlayerNum;

	SuicideObj::CCollisionObj m_collision;
};

