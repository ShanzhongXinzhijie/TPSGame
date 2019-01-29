#include "stdafx.h"
#include "Fade.h"


Fade::Fade() {
	sprite.Init(L"Resource/spriteData/Black.dds");
}


Fade::~Fade() {
}

void Fade::PostRender() {
	if (state == In) {
		alpha += fadePerSec * GetDeltaTimeSec();
		if (1.0f <= alpha) {
			alpha = 1.0f;
			state = Idle;
			func();
			func = nullptr;
		}
	} else if(state == Out){
		alpha -= fadePerSec * GetDeltaTimeSec();
		if (alpha <= 0.0f) {
			alpha = 0.0f;
			state = Idle;
		}
	}
	sprite.Draw(CVector2::Zero(), { 1280.0f, 720.0f }, CVector2::Zero(), 0.0f, {1.0f, 1.0f, 1.0f, alpha});
}

void Fade::fadeOut() {
	if (state == Idle) {
		state = Out;
	}
}

void Fade::fadeIn(const std::function<void()>& function) {
	if (state == Idle) {
		state = In;
		func = function;
	}
}


