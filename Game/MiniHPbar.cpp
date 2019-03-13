#include "stdafx.h"
#include "MiniHPbar.h"


MiniHPbar::MiniHPbar(float maxHp): c_maxHp(maxHp) {
	outImg.Init(L"Resource/spriteData/MiniHpOut.dds");
	inImg.Init(L"Resource/spriteData/MiniHpIn.dds");
	backImg.Init(L"Resource/spriteData/MiniHpBack.dds");
}


MiniHPbar::~MiniHPbar() {
}

void MiniHPbar::Update() {
	if (drawTime > 0.0f) {
		drawTime -= GetDeltaTimeSec();
	}
}

void MiniHPbar::PostRender() {
	if (drawTime > 0.0f) {
		float alpha = 1.0f;
		if (drawTime < 0.5f) {
			alpha = drawTime;
		}

		float hpScale = m_hp / c_maxHp;

		CVector2 pos = m_pos;
		pos.x -= 44 * m_scale;

		backImg.DrawScreenPos(pos, { m_scale, m_scale }, { 0.0f,0.5f }, 0.0f, {1, 1, 1, alpha});
		inImg.DrawScreenPos(pos, { m_scale*hpScale, m_scale }, { 2.0f/88,0.5f }, 0.0f, { 1, 1, 1, alpha });
		outImg.DrawScreenPos(pos, { m_scale, m_scale }, { 0.0f,0.5f }, 0.0f, { 1, 1, 1, alpha });
	}
}
