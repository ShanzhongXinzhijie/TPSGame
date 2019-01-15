#pragma once
class SqSpawner {
public:
	SqSpawner(float side = 1000);
	~SqSpawner();

	void setPos(const CVector3& pos) {
		m_pos = pos;
	}

	void setSide(float side) {
		m_side = side;
	}

	CVector3 getPos();

private:
	CVector3 m_pos;
	float m_side;


	std::mt19937 random;
};

