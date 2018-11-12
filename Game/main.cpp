#include "stdafx.h"

#include "Title.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitEngineParameter initparam;
	GetEngine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game", initparam);	

	//CPlayer plyer(0);
	//PlayersManager playerMng;
	//Ground ground;
	new Title;
	//ゲームループ。
	GetEngine().RunGameLoop();
}