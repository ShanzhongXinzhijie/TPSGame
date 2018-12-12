#include "stdafx.h"
#include "HPbar.h"


HPbar::HPbar(){
	m_outSpr.Init(L"Resource/spriteData/HP_out.dds");
	m_inSpr.Init(L"Resource/spriteData/HP_in.dds");
}


HPbar::~HPbar() {
}

void HPbar::Draw(int hp, int maxHP) {
	m_outSpr.DrawScreenPos(m_pos, CVector2::One(), {0.992f,0.0f});
	m_inSpr.DrawScreenPos(m_pos, {(float)hp/maxHP ,1.0f}, { 0.992f,0.0f });
}
