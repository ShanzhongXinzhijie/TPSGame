#pragma once
#include "NetworkConst.h"
#include "NetPlayerCaster.h"
#include "CPlayer.h"
#include "CitizenGene.h"
#include <queue>

struct OnlinePlayerStatus {
	bool m_isNoReceive = true;
	int m_cnt = INT_MIN;

	ActionSender m_actionSender;
	bool m_isFly = false;
	bool m_isUpd8FlyTimer = false;  float m_flyTimer = 0.0f;
	bool m_isUpd8BulletCnt = false; int m_bulletCnt[CPlayer::weaponNum] = {};
	bool m_isUpdatePos = false;		CVector3 m_pos; 
	bool m_isUpd8Velocity = false;  CVector3 m_velocity;
	bool m_isUpdateDead = false;	bool m_isDead = false;
};

class NetPlayerReceiver : public IQSGameObject
{
public:
	NetPlayerReceiver();
	~NetPlayerReceiver();

	void PreUpdate()override;
	void PostLoopUpdate()override;

	//�C�x���g��M
	void EventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj);	
	//�C�x���g���s
	void RunEvent(int playerNr, bool frameSkip = false);
	//�v���C���[�ɏ��n��
	void UpdatePlayer(int playerNr);
	//�s���ɏ��n��
	void UpdateCitizen();

	//�v���C���[�ݒ�
	void SetPlayer(CPlayer* pCPlayer, NetPlayerCaster* pCaster) {
		m_pCPlayer[pCPlayer->playerNum] = pCPlayer;
		m_pCaster[pCPlayer->playerNum] = pCaster;
		//���̎��_�܂ł̏��n��
		UpdatePlayer(pCPlayer->playerNum);
	}
	//�v���C���[����
	void RemovePlayer(int plyNum) {
		m_pCPlayer[plyNum] = nullptr;
		m_pCaster[plyNum] = nullptr;
		m_status[plyNum] = OnlinePlayerStatus();
		//�L���[���N���A
		std::queue<std::tuple<NetworkEventCode, int, ExitGames::Common::Hashtable>>().swap(m_eventContentQueue[plyNum]);
	}

	//�s���ݒ�
	void SetCitizenGene(CitizenGene* ptr) {
		m_citizenGene = ptr;
		if (m_citizenGene) {
			//���̎��_�܂ł̏��n��
			UpdateCitizen();
		}
		else {
			//�X�e�[�^�X���N���A
			m_citizensStatus.clear();
		}
	}

private:
	CPlayer* m_pCPlayer[NET_MAX_PLAYER + 1] = { nullptr };
	NetPlayerCaster* m_pCaster[NET_MAX_PLAYER + 1] = { nullptr };
	OnlinePlayerStatus m_status[NET_MAX_PLAYER + 1];

	CitizenGene* m_citizenGene = nullptr;
	struct CitizensStatus {
		int timeCnt = INT_MIN;
		int plyNum = -1;
		CVector3 pos;
	};
	std::unordered_map<int, CitizensStatus> m_citizensStatus;

	std::queue<std::tuple<NetworkEventCode, int, ExitGames::Common::Hashtable>> m_eventContentQueue[NET_MAX_PLAYER + 1];
};

