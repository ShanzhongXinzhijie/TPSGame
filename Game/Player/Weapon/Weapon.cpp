#include "stdafx.h"
#include "Weapon.h"

Weapon::Weapon(CPlayer* player, GameObj::CSkinModelRender* playerModel)
	: player(player), playerModel(playerModel){
}


Weapon::~Weapon() {
}
