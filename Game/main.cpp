#include "stdafx.h"
#include "Title.h"
#include "Network/Network.h"
#include "Fade.h"
#include "CConfig.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
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
			//���[�O���t�B�b�N�X�ݒ�
			initparam.SetLowSpecSetting();
		}
	}

	GetEngine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game", initparam);

	SetIsDebugDraw(true);//fps�Ƃ��\��
	//SetPhysicsDebugDrawMode(btIDebugDraw::DBG_DrawWireframe);//����̕\��

	NetWorkManager networkManager;
	networkManager.SetName(L"NetWorkManager");

	CFont::LoadDefaultFont(L"Preset/Font/default.spritefont");

	Fade fade;

	Title* T = new Title(&fade); T->SetName(L"TitleClass");

	//�Q�[�����[�v�B
	GetEngine().RunGameLoop();

	//�R���t�B�O�Z�[�u
	T = FindGO<Title>(L"TitleClass");
	if (T) { T->ConfigSave(); }
}