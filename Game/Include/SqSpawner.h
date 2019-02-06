#pragma once
class SqSpawner {
public:
	SqSpawner(float side, const CVector3& pos);
	~SqSpawner();

	void setPos(const CVector3& pos) {
		m_pos = pos;
	}

	float getSide() {
		return m_side;
	}

	CVector3 getPos();

private:
	CVector3 m_pos;
	float m_side;


	std::mt19937 random;
};

