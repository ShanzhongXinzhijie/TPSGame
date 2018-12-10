#include "stdafx.h"
#include "ConfirmPlayers.h"
#include "CPlayer.h"
#include "Title.h"
#include "Game.h"

#include "Network/Network.h"

ConfirmPlayers::ConfirmPlayers() : list(L"Resource/spriteData/waku.dds") {
	camera.SetPos({ 0, 50, 200 });
	camera.SetTarget({ 0, 50, 0 });
	camera.UpdateMatrix();

#ifdef SpritScreen
	GetCameraList().clear();
	GetCameraList().push_back(&camera);
	GetCameraList().push_back(&camera);
#else
	SetMainCamera(&camera);
#endif
}


ConfirmPlayers::~ConfirmPlayers() {
}

bool ConfirmPlayers::Start() {

	m_dirlight.SetColor({ 1,1,1 });
	m_dirlight.SetDirection({ 0,0,1 });

	m_sprite.Init(L"Resource/spriteData/Title.dds");

	m_netWork = FindGO<NetWorkManager>(L"NetWorkManager");

#ifndef SpritScreen
	//�v���C���[�l�[�����߂�
	std::wstring wstr;
	float randam = CMath::RandomZeroToOne();
	float min = 0.0f;
	for (int i = 0; i < 8; i++) {
		if (randam < min + 1.0f / 8.0f) {
			switch (i)
			{
			case 0:
				wstr = L"�G���t";
				break;
			case 1:
				wstr = L"���C����";
				break;
			case 2:
				wstr = L"�E�B�b�`";
				break;
			case 3:
				wstr = L"�h���S��";
				break;
			case 4:
				wstr = L"�l�N��";
				break;
			case 5:
				wstr = L"���@���p�C�A";
				break;
			case 6:
				wstr = L"�r�V���b�v";
				break;
			case 7:
				wstr = L"�l���V�X";
				break;
			default:
				break;
			}
			wstr += L"����";
			break;
		}
		min += 1.0f / 8.0f;
	}
	//�T�[�o�[�ƃ��[���ɐڑ�
	m_netWork->ConnectJoin(wstr.c_str());
#endif

	return true;
}

void ConfirmPlayers::Update() {
#ifdef SpritScreen
	for (int num = 1; num < 4; num++) {

		if (Pad(num).GetDown(enButtonStart)) {
			if (players.count(num) == 0) {
				players[num] = { 1,0,0,1 };
			} else {
				players.erase(num);
			}
		}
	}
#endif

	if (Pad(0).GetDown(enButtonStart)) {
#ifdef SpritScreen
		GetCameraList().clear();
#endif
		bool isReady = true;
#ifndef SpritScreen
		//���[���ɓ���̂�҂�
		if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) { isReady = false; }
#endif
		if (isReady) {
			new Game();
			delete this;
		}
	} else if (Pad(0).GetDown(enButtonBack)) {
		new Title;
		delete this;
	}
}

void ConfirmPlayers::PostRender() {
	m_sprite.Draw({ 0.5f, 0.5f }, CVector2::One(), { 0.5f, 0.5f });

	if (GetPhoton()->GetState() == PhotonNetworkLogic::JOINED) {
		//���[�����̃v���C���[��\��
		const ExitGames::Common::JVector<ExitGames::LoadBalancing::Player*>& players = GetPhoton()->GetPlayers();
		for (unsigned int i = 0; i < players.getSize(); i++){
			list.values.emplace_back(players[i]->getName().cstr());
			list.values.back() += L" :"; list.values.back() += std::to_wstring(players[i]->getNumber());
			if (players[i]->getNumber() == GetPhoton()->GetLocalPlayerNumber()) {
				list.values.back() += L"  <<����";
			}
		}
	}
	else {
		list.values.emplace_back(L"�ڑ���...");
	}

	list.Draw();

	list.values.clear();
}
