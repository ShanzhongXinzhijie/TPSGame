#include "stdafx.h"

#include "CStage.h"
#include "CPlayer.h"

class tester : public IGameObject {
public:
	void Update()override {
		if (GetAsyncKeyState(VK_TAB)) {
			GetEngine().GetGraphicsEngine().GetAmbientOcclusionRender().SetEnable(false);
			//ライト作成
			if (!m_pl) { 
				m_pl = new GameObj::CPointLight({ 0.0f, 1.0f ,0.0f }, { 0.0f,10.0f ,0.0f }, 250.0f, 1.0f);
				m_recever = new GOStatusReceiver;
				m_pl->AddStatusReceiver(m_recever);
			}
		}
		else {
			GetEngine().GetGraphicsEngine().GetAmbientOcclusionRender().SetEnable(true);
		}
		if (GetAsyncKeyState(VK_DELETE)) {
			//ライト削除
			if (m_pl) { 
				delete m_pl;m_pl = nullptr;
			}
		}

		if (GetAsyncKeyState(VK_BACK)) {
			delete m_recever; m_recever = nullptr;
		}
		if (m_recever && m_recever->GetStatus().m_isDead) {
			m_pl = m_pl;
		}
	}

private:
	GameObj::CPointLight* m_pl = nullptr;
	GOStatusReceiver* m_recever = nullptr;
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitEngineParameter initparam; //initparam.frameBufferWidth *= 2; initparam.frameBufferHeight *= 2;//initparam.limitFps = 25; initparam.standardFps = 30; //initparam.refleshRate = 120;
	GetEngine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game", initparam);	
	//GetEngine().GetGraphicsEngine().SetUseVSync(true);
	//GetEngine().SetUseFpsLimiter(false);

	CPlayer plyer;
	//CStage stage;
	tester tester;
	//AddGO(&plyer);
	//AddGO(&stage);
	//AddGO(&tester);

	//ゲームループ。
	GetEngine().RunGameLoop();
}