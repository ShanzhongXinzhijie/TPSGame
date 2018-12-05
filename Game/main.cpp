#include "stdafx.h"

#include "Title.h"

#include "Network/Network.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitEngineParameter initparam;
	initparam.isSplitScreen = enSide_TwoSplit;
	GetEngine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game", initparam);

	NetWorkManager networkManager;

	new Title;

	//�Q�[�����[�v�B
	GetEngine().RunGameLoop();
}