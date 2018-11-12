#include "stdafx.h"
#include "ActionSender.h"


ActionSender::ActionSender(CVector2 movePal, bool jumpPal, bool dashPal)
	: movement(movePal), jump(jumpPal) , dash(dashPal){
}

ActionSender::~ActionSender() {
}
