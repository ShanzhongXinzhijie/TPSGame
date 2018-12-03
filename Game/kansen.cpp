#include "stdafx.h"
#include "kansen.h"


kansen::kansen(std::vector<CPlayer*>& ps) : players(ps)
{
}


kansen::~kansen()
{
}

void kansen::Update(bool isOnGround)
{
	
}

bool kansen::isAtk()
{
	return Atk;
}

CVector3 kansen::getMove()
{
	return CVector3();
}

CQuaternion kansen::getTurn()
{
	return CQuaternion();
}
