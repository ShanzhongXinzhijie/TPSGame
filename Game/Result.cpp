#include "stdafx.h"
#include "Include\Result.h"
#include "Title.h"


Result::Result(unsigned int r, unsigned int b): red(r), blue(b) {
}


Result::~Result() {
}

bool Result::Start() {
	camera.SetPos({ 0, 0, -100 });
	camera.SetTarget({ 0, 0, 0 });
	camera.UpdateMatrix();
#ifdef SpritScreen
	GetCameraList().clear();
	GetCameraList().push_back(&camera);
	GetCameraList().push_back(&camera);
#else
	SetMainCamera(&camera);
#endif

	m_sprite.Init(L"Resource/spriteData/Title.dds");
	return true;
}

void Result::Update() {
	if (Pad(0).GetDown(enButtonA)) {
		delete this;
		new Title;
	}
}

void Result::PostRender() {
	m_sprite.Draw({ 0.5f, 0.5f }, CVector2::One(), { 0.5f, 0.5f });

	m_font.Draw(L"Œ‹ ‰Ê ”­ •\", { 0.47f,0.3f }, CVector4::White(), { 3.0f,3.0f }, { 0.5f, 0.5f });
	{
		wchar_t countDisp[20];
		swprintf_s(countDisp, L"Ô %d‘Ì  Â %d‘Ì", red, blue);
		m_font.Draw(countDisp, { 0.47f,0.5f }, CVector4::White(), { 2.0f,2.0f }, { 0.5f, 0.5f });
	}
	const wchar_t* disp;
	if (red == blue) {
		disp = L"ˆø‚«•ª‚¯";
	} else if(red > blue) {
		disp = L"Ô‚ÌŸ‚¿";
	} else {
		disp = L"Â‚ÌŸ‚¿";
	}
	m_font.Draw(disp, { 0.47f, 0.8f }, CVector4::White(), { 1.5f,1.5f }, { 0.5f, 0.5f });
}
