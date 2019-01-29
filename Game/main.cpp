#include "stdafx.h"
#include "Title.h"
#include "Network/Network.h"
#include "Fade.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitEngineParameter initparam;

#ifdef SpritScreen
	initparam.isSplitScreen = enSide_TwoSplit;
#else
	initparam.isSplitScreen = enNoSplit;
#endif

	GetEngine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game", initparam);

	NetWorkManager networkManager;
	networkManager.SetName(L"NetWorkManager");

	Fade fade;

	new Title(&fade);

	//ゲームループ。
	GetEngine().RunGameLoop();
}