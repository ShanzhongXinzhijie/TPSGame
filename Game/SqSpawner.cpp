#include "stdafx.h"
#include "Include\SqSpawner.h"


SqSpawner::SqSpawner() {
}


SqSpawner::~SqSpawner() {
}

CVector3 SqSpawner::getPos() {
	CVector3 pos;
	pos.x = (random() % (int)(maxPos.x - minPos.x)) + minPos.x;
	pos.z = (random() % (int)(maxPos.z - minPos.z)) + minPos.z;
	pos.y = maxPos.y;
	return pos;
}
