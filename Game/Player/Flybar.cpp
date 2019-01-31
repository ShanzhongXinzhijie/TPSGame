#include "stdafx.h"
#include "Flybar.h"


Flybar::Flybar(){
	m_outSpr.Init(L"Resource/spriteData/FlyBar_Out.dds");
	m_inSpr.Init(L"Resource/spriteData/FlyBar_In.dds");
}


Flybar::~Flybar(){
}

void Flybar::Draw(float flypower, float maxflypower) {
	m_outSpr.DrawScreenPos(m_pos, CVector2::One(), { 0.992f,0.0f });
	m_inSpr.DrawScreenPos(m_pos, { flypower / maxflypower ,1.0f }, { 0.992f,0.0f });
}
