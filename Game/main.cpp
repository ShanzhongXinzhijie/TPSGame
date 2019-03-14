#include "stdafx.h"
#include "Title.h"
#include "Network/Network.h"
#include "Fade.h"
#include "CConfig.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitEngineParameter initparam;

	initparam.variableFpsMaxSec = 10.0f;

#ifdef SpritScreen
	initparam.isSplitScreen = enSide_TwoSplit;
#else
	initparam.isSplitScreen = enNoSplit;
#endif

	{
		CConfig configLoad;
		configLoad.Load();
		if (configLoad.GetConfigData().lowGraphics) {
			//ローグラフィックス設定
			initparam.SetLowSpecSetting();
		}
	}

	GetEngine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game", initparam);

	SetIsDebugDraw(true);//fpsとか表示
	//SetPhysicsDebugDrawMode(btIDebugDraw::DBG_DrawWireframe);//判定の表示

	NetWorkManager networkManager;
	networkManager.SetName(L"NetWorkManager");

	CFont::LoadDefaultFont(L"Preset/Font/default.spritefont");

	Fade fade;

	Title* T = new Title(&fade); T->SetName(L"TitleClass");

	//ゲームループ。
	GetEngine().RunGameLoop();

	//コンフィグセーブ
	T = FindGO<Title>(L"TitleClass");
	if (T) { T->ConfigSave(); }
}