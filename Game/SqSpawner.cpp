#include "stdafx.h"
#include "Include\SqSpawner.h"


SqSpawner::SqSpawner(float s, const CVector3& pos) : m_side(s), m_pos(pos), random(){
}


SqSpawner::~SqSpawner() {
}

CVector3 SqSpawner::getPos() {
	CVector3 pos = m_pos;
	pos.x += (random() % (int)(m_side)) - m_side/2;
	pos.z += (random() % (int)(m_side)) - m_side/2;
	return pos;
}
