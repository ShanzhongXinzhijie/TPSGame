#include "stdafx.h"
#include "ActionSender.h"


ActionSender::ActionSender(CVector2 movePal, bool jumpPal, bool dashPal,
						   CVector3 lookPal, bool shotPal, bool reloadPal)
	: movement(movePal), jump(jumpPal) , dash(dashPal), lookVec(lookPal), shot(shotPal), reload(reloadPal){
}

ActionSender::~ActionSender() {
}
