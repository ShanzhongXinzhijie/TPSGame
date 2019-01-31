#include "stdafx.h"
#include "GuiList.h"


GuiList::GuiList(const wchar_t* backImagePath) {
	back.Init(backImagePath);
}


GuiList::~GuiList() {
}

void GuiList::Draw() {
	back.Draw(position, scale, pivot, 0, color, DirectX::SpriteEffects_None, depth);
	CVector2 valuePos = position;
	valuePos.x -= pivot.x * 800.0f / 1280.0f - 40.0f/1280.0f;
	valuePos.y -= pivot.y * 600.0f / 720.0f - 40.0f/720.0f;
	unsigned int high = 0;
	for (const std::wstring& v : values) {
		font.Draw(v.c_str(), valuePos, color, scale, CVector2::Zero(), 0.0f, DirectX::SpriteEffects_None, depth);
		valuePos.y += 40.0f / 720.0f;
	}
}
