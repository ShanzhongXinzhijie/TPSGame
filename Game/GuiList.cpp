#include "stdafx.h"
#include "GuiList.h"


GuiList::GuiList(const wchar_t* backImagePath) {
	back.Init(backImagePath);
}


GuiList::~GuiList() {
}

void GuiList::Draw() {
	back.Draw(position, scale, pivot, 0, color);
	CVector2 valuePos = position;
	valuePos.x -= pivot.x * 800.0f / 1280.0f - 40.0f/1280.0f;
	valuePos.y -= pivot.y * 600.0f / 720.0f - 40.0f/720.0f;
	unsigned int high = 0;
	for (const wchar_t* v : values) {
		font.Draw(v, valuePos, color, scale);
		valuePos.y += 40.0f / 720.0f;
	}
}
