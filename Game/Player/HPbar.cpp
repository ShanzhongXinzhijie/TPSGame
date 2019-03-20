#include "stdafx.h"
#include "HPbar.h"


HPbar::HPbar(float maxHp, float maxFly) : maxHp(maxHp), maxFly(maxFly){
	m_outSpr.Init(L"Resource/spriteData/HP_out.dds");
	m_inSpr.Init(L"Resource/spriteData/HP_in.dds");
	m_inFly.Init(L"Resource/spriteData/Fly_in.dds");
}


HPbar::~HPbar() {
}

void HPbar::Draw(float hp, float fly, bool flyRest) {
	m_outSpr.DrawScreenPos(m_pos, CVector2::One(), {0.992f,0.0f});
	m_inSpr.DrawScreenPos(m_pos, {hp/maxHp ,1.0f}, { 0.992f,0.0f });
	if (flyRest) {
		m_inFly.DrawScreenPos(m_pos, { 1.0f,1.0f }, { 0.992f,0.0f }, 0.0f, {1,0,bright,1});
		bright += GetDeltaTimeSec() * brightMove;
		if (bright <= 0.0f || 1.0f <= bright) {
			brightMove *= -1;
		}
		recoverWhite = 0.3f;
	} else {
		const float c = recoverWhite*10+1;
		m_inFly.DrawScreenPos(m_pos, { fly / maxFly ,1.0f }, { 0.992f,0.0f }, 0.0f, {c,c,c,1});
		if (recoverWhite > 0.0f) {
			recoverWhite -= GetDeltaTimeSec();
			if (recoverWhite < 0.0f) {
				recoverWhite = 0.0f;
			}
		}
	}
}
