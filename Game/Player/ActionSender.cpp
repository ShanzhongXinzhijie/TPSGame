#include "stdafx.h"
#include "ActionSender.h"


ActionSender::ActionSender(CVector2 move, bool jump, bool dash,
						   CVector3 look, bool shot, bool reload)
	: movement(move), jump(jump) , dash(dash), lookVec(look), shot(shot),
	reload(reload){
}

ActionSender::~ActionSender() {
}
