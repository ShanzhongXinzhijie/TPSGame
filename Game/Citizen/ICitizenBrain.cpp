#include "stdafx.h"
#include "ICitizenBrain.h"


ICitizenBrain::ICitizenBrain() {
}


ICitizenBrain::~ICitizenBrain() {
}

bool ICitizenBrain::isAtk() {
	return false;
}

void ICitizenBrain::SetCitizen(Citizen* setCitizen)
{
	citizen = setCitizen;
}
