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
	if (!isOnGround) {
		jumpPower -= 980.0f * GetDeltaTimeSec();
	}
	
}

bool kansen::isAtk()
{
	return Atk;
}

CVector3 kansen::getMove()
{
	CVector3 walk;
	for (CPlayer* player : players) {
		CVector3 kyori;
		kyori = player->getPosition() - citizen_pos;
		float kaiten = atan2f(kyori.x, kyori.z);
		
		if (kyori.Length() < 300.0f) {
			walk += kyori;
			rot.SetRotation(CVector3::AxisY(), kaiten);
			break;
		}
	}
	
	walk.y = jumpPower;
	return walk;
}

CQuaternion kansen::getTurn()
{
	
	return rot;
}
