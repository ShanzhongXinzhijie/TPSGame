#include "stdafx.h"
#include "ActionSender.h"


ActionSender::ActionSender(CVector2 move, bool jump, bool dash,
						   CVector3 look, bool shot, bool reload, bool fly)
	: movement(move), jump(jump) , dash(dash), lookVec(look), shot(shot),
	reload(reload), fly(fly){
}

ActionSender::~ActionSender() {
}
