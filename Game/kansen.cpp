#include "stdafx.h"
#include "kansen.h"


kansen::kansen(std::vector<CPlayer*>& ps,const CVector3& Citizenpos) : players(ps),citizen_pos(Citizenpos)
{
}


kansen::~kansen()
{
}

void kansen::Update(bool isOnGround)
{
	for (CPlayer* player : players) {
		CVector3 kyori;
		kyori = player->getPosition() - citizen_pos;
		
		if (kyori.Length() < 100.0f) {
			kyori += kyori;
		}
	}
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
