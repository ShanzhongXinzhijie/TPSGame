#include "stdafx.h"

#include "Title.h"

#include "Network/Network.h"

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

	new Title;

	//ゲームループ。
	GetEngine().RunGameLoop();
}