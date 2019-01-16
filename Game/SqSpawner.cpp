#include "stdafx.h"
#include "Include\SqSpawner.h"


SqSpawner::SqSpawner(float s) : m_side(s), random(){
}


SqSpawner::~SqSpawner() {
}

CVector3 SqSpawner::getPos() {
	CVector3 pos = m_pos;
	pos.x += (random() % (int)(m_side)) - m_side/2;
	pos.z += (random() % (int)(m_side)) - m_side/2;
	return pos;
}
