#include "stdafx.h"
#include "kansen.h"


kansen::kansen(const std::unordered_map<int, CPlayer*>& pm,const CVector3& Citizenpos,Team*& teamm)
	: playersMap(pm),citizen_pos(Citizenpos),team(teamm)
{
}


kansen::~kansen()
{
}

void kansen::Update(bool isOnGround, float deltaTime)
{
	if (!isOnGround) {
		jumpPower -= 980.0f * deltaTime;
	}
	walk = { 0.0f,0.0f,0.0f };
	for (const std::pair<int, CPlayer*>playerPair : playersMap) {
		CPlayer* player = playerPair.second;
		if (!team->hasPlayer(player)) {
			CVector3 kyori;
			kyori = player->getPosition() - citizen_pos;
			float kaiten = atan2f(kyori.x, kyori.z);

			if (kyori.Length() < 50.0f) {
				Atk = true;
			}
			else {
				Atk = false;
			}
			if (kyori.Length() < 300.0f) {
				walk = kyori;
				rot.SetRotation(CVector3::AxisY(), kaiten);
				break;
			}
		}
	}
	walk.y = jumpPower;
	
}

bool kansen::isAtk()
{
	//for (const std::pair<int, CPlayer*>playerPair : playersMap) {
	//	CPlayer* player = playerPair.second;
	//	if (!team->hasPlayer(player)) {
	//		CVector3 kyori;
	//		kyori = player->getPosition() - citizen_pos;
	//		float kaiten = atan2f(kyori.x, kyori.z);

	//		if (kyori.Length() < 50.0f) {
	//			Atk = true;
	//		}
	//		else {
	//			Atk = false;
	//		}
	//	}
	//}
	return Atk;
}

CVector3 kansen::getMove()
{
	/*CVector3 walk;
	for (const std::pair<int, CPlayer*> playerPair : playersMap) {
		CPlayer* player = playerPair.second;
		if (!team->hasPlayer(player)) {
			CVector3 kyori;
			kyori = player->getPosition() - citizen_pos;
			float kaiten = atan2f(kyori.x, kyori.z);

			if (kyori.Length() < 300.0f) {
				walk += kyori;
				rot.SetRotation(CVector3::AxisY(), kaiten);
				break;
			}
		}
	}
	
	*/
	return walk;
}

CQuaternion kansen::getTurn()
{
	
	return rot;
}
