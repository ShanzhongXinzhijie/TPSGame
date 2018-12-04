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
	unsigned int high = 0;
	for (const wchar_t* v : values) {
		font.Draw(v, valuePos, color, scale);
		valuePos.y += 20.0f;
	}
}
