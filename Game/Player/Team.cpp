#include "stdafx.h"
#include "Team.h"
#include "PlayerGene.h"

Team::Team(CVector4 col, const wchar_t* n, PlayerGene* generator): color(col), name(n), generator(generator){
}


Team::~Team() {
}

CVector3 Team::getHome() const {
	return generator->getHome(this);
}
