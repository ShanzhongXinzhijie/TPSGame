#include "stdafx.h"
#include "Game.h"
#include "Ground.h"
#include "CircleWalk.h"
#include "sikakukuidou.h"
#include "ziguzagu.h"
#include "ittarikitari.h"
#include "Result.h"
#include "Fade.h"
#include "GameWaiter.h"

Game::Game(Fade* fade, float timeLimit, int citizenCnt, int seed, int startTime_ms) : citizenGene(this), timer(timeLimit){
	this->fade = fade;
	fade->fadeOut();

	level.Init(L"Resource/Level/level.tkl", [&](LevelObjectData& objData)->bool {
		if (objData.EqualObjectName(L"player700")) {
			playerGene.addSpawner(objData.position, 780.0f);
		} else if (objData.EqualObjectName(L"citizen600")) {
			citizenGene.addSpawner(objData.position, 600.0f);
		} else if (objData.EqualObjectName(L"citizen1800")) {
			citizenGene.addSpawner(objData.position, 1800.0f);
		}
		return true;
	});
	ground = new Ground(CVector3::Zero());

	bgm = NewGO<SuicideObj::CBGM>(L"Resource/sound/BGM_battle.wav");
	bgm->Play(false, true);

#ifdef SpritScreen
	createPlayer(true, 0);
	createPlayer(true, 1);
#else
	//�v���C���[�}�l�[�W���[�̏�����(�v���C���[�̍쐬)
	m_netPlayerManager.Init(this);
#endif

	//��������
	srand(seed);
	//�s���쐬
	citizenGene.createCitizen(citizenCnt);

#ifdef SpritScreen
	karicamera.SetPos({ 600,600,600 });
	karicamera.SetTarget({0, 0, 0});
	karicamera.SetFar(3000);
	GetCameraList().push_back(&karicamera);
	GetCameraList().push_back(&karicamera);
#endif

#ifndef SpritScreen
	m_netWork = FindGO<NetWorkManager>(L"NetWorkManager");

	//�Q�[���J�n�^�C�}�[�ݒ�
	int t = GetPhoton()->GetSeverTime_ms(); 
	if (t < 0 && startTime_ms >= 0) { startTime_ms = INT_MIN; }
	m_waitGameStartTimer_sec = CMath::Clamp(15.0f - (t - startTime_ms)*0.001f, 0.0f, 15.0f);
	//�Q�[�����E�F�C�g��Ԃɂ���
	if (m_waitGameStartTimer_sec > 0.0f) { GameWaiter::SetIsWait(true); }
#endif

	//���̃t���[���̉σt���[������
	//GetEngine().UnableVariableFramerateOnce();
}

Game::~Game() {
	bgm->Stop();
	delete ground;
}

void Game::Update() {
	if (m_waitGameStartTimer_sec > 0.0f) {
		m_waitGameStartTimer_sec -= GetDeltaTimeSec();
		if (m_waitGameStartTimer_sec <= 0.0f) {
			m_waitGameStartTimer_sec = 0.0f;
			//�E�F�C�g��Ԃ̉���
			GameWaiter::SetIsWait(false);
		}
		else {
			return;
		}
	}
	if (timer > 0) {
		timer -= GetDeltaTimeSec();
	}
	if (timer <= 0) {
		timer = 0;
#ifdef SpritScreen
		fade->fadeIn([&]() {
			new Result(playerGene, fade);
			delete this; return;
		});
#else
		if (GetPhoton()->GetIsMasterClient()) {
			//�}�X�^�[�N���C�A���g
			
			//�]���r�̐��Z�b�g
			int zombieCnt[NET_MAX_PLAYER + 1];
			std::vector<Team*> teamPtrs = playerGene.getTeams();
			for (int i = 0; i < min(teamPtrs.size(), NET_MAX_PLAYER + 1); i++) {
				zombieCnt[i] = teamPtrs[i]->getZombieCount();
			}
			//�I�����ߑ��M
			m_netEventCaster.SendGameEnd(zombieCnt);

			//���U���g��
			fade->fadeIn([&]() {
				new Result(playerGene, fade);
				delete this; return;
			});
		}
#endif
	}	

#ifndef SpritScreen
	if (!GetPhoton()->GetIsMasterClient()) {
		//�ʏ�N���C�A���g

		//�I�����ߗ��Ă���I��
		if (m_netWork->GetNetEventReceiver().GetGameState() == NetGameEventReceiver::enGameEnd) {

			//�]���r�̐�����
			std::vector<Team*> teamPtrs = playerGene.getTeams();
			for (int i = 0; i < min(teamPtrs.size(), NET_MAX_PLAYER + 1); i++) {
				teamPtrs[i]->setZombie(m_netWork->GetNetEventReceiver().GetGameEndData().m_zombieCnt[i]);
			}			

			//���U���g��
			fade->fadeIn([&]() {
				new Result(playerGene, fade);
				delete this; return;
			});
		}
	}
#endif

}

void Game::PostRender() {

	if (m_waitGameStartTimer_sec > 0.0f) {
		wchar_t countDisp[24];
		swprintf_s(countDisp, L"�J�n�܂�...%.1f�b", m_waitGameStartTimer_sec);
		font.Draw(countDisp, { 0.5f, 0.5f }, CVector4::White(), CVector2::One(), { 0.5f,0.5f },0.0f,DirectX::SpriteEffects_None,0.0f);
		return;
	}

	wchar_t countDisp[10];
	swprintf_s(countDisp, L"�c��%.1f�b", timer);
	font.Draw(countDisp, { 0.1f, 0.05f });
}

void Game::createPlayer(bool isMe, int playerNum) {
	playerGene.createPlayer(isMe, playerNum);
}

void Game::removePlayer(int playerNum) {
	playerGene.removePlayer(playerNum);
}

