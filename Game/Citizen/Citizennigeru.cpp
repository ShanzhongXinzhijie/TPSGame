#include "stdafx.h"
#include "Citizennigeru.h"


Citizennigeru::Citizennigeru(const std::unordered_map<int, CPlayer*>& pm) : playersMap(pm)
{
}


Citizennigeru::~Citizennigeru()
{
}

void Citizennigeru::Update(bool isOnGround, float deltaTime)
{
	if (!isOnGround) {
		jumpPower -= 980.0f * deltaTime;
	}
	else {
		if (jumpPower < 0) {
			jumpPower = 0;
		}
	}
	citizen->getPos();
	for (const std::pair<int, CPlayer*>playerPair : playersMap) {
		CPlayer* player = playerPair.second;

		CVector3 kyori;
		kyori = citizen->getPos() - player->getPosition();
		float kaiten = atan2f(kyori.x, kyori.z);

		if (kyori.Length() < 300.0f) {
			CVector3 kyori2;
			kyori2 = citizen->getPos() - player->getPosition();
			kyori2.Normalize();
			kyori2 *= 500;
			walk = kyori2;
			rot.SetRotation(CVector3::AxisY(), kaiten);

#ifndef SpritScreen
			//Ž©•ª‚©‚ç“¦‚°‚é‚È‚çˆÊ’u“¯Šú
			if (player->playerNum == GetPhoton()->GetLocalPlayerNumber()) {
				citizen->SetIsSend(true);
				citizen->SetIsAvg(false);
				citizen->SetTargetPly(player->playerNum);
				citizen->SetTargetCnt(player->GetNetCaster()->GetCnt());
			}
#endif
			break;
		}
	}
	walk.y = jumpPower;
}
CVector3 Citizennigeru::getMove()
{
	return walk;
}

CQuaternion Citizennigeru::getTurn()
{
	return rot;
}
