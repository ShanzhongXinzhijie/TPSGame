#pragma once
class SqSpawner {
public:
	SqSpawner();
	~SqSpawner();

	void setMaxPos(const CVector3& pos) {
		maxPos = pos;
	}

	void setMinPos(const CVector3& pos) {
		minPos = pos;
	}

	CVector3 getPos();

private:
	CVector3 maxPos;
	CVector3 minPos;


	std::mt19937 random;
};

