#include "stdafx.h"
#include "MiniHPbar.h"
#include "Game.h"

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
		CVector3 vec1 = GetMainCamera()->GetTarget() - GetMainCamera()->GetPos();
		CVector3 vec2 = m_pos - GetMainCamera()->GetPos();

		if (vec1.Dot(vec2) < 0) {
			return;
		}

		CVector2 pos2D = GetMainCamera()->CalcScreenPosFromWorldPosScreenPos(m_pos);

		const float cameraAngle = Game::getGame()->getMainPlayer()->getCamera().getAngle();
		const float tan = tanf(cameraAngle*0.5f) * 2;
		const float scale = 400.0f / (vec2.Length()*tan);

		float a = 1.0f;
		if (drawTime < 0.5f) {
			a = drawTime/0.5f;
		}

		const float hpScale = m_hp / c_maxHp;

		pos2D.x -= 88 * scale;

		backImg.DrawScreenPos(pos2D, { scale, scale }, { 0.0f,0.5f }, 0.0f, {a, a, a, a}, DirectX::SpriteEffects_None, 0.7f);
		inImg.DrawScreenPos(pos2D, { scale*hpScale, scale }, { 2.0f/176,0.5f }, 0.0f, { a, a, a, a }, DirectX::SpriteEffects_None, 0.7f);
		outImg.DrawScreenPos(pos2D, { scale, scale }, { 0.0f,0.5f }, 0.0f, { a, a, a, a }, DirectX::SpriteEffects_None, 0.7f);
	}
}
