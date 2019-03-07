#include "stdafx.h"
#include "NormalBullet.h"
#include "CPlayer.h"
#include "Citizen.h"

using SuicideObj::CCollisionObj;

NormalBullet::NormalBullet(CPlayer* player,CVector3 position, CVector3 direction, const wchar_t* modelPath)
	: Bullet(player, position, direction, modelPath){
}


NormalBullet::~NormalBullet() {

}