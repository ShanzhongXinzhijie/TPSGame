#include "stdafx.h"

#include "Title.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitEngineParameter initparam;
	initparam.isSplitScreen = enSide_TwoSplit;
	GetEngine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game", initparam);

	new Title;

	//ゲームループ。
	GetEngine().RunGameLoop();
}