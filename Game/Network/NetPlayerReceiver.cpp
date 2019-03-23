#include "stdafx.h"
#include "NetPlayerReceiver.h"
#include "Citizen.h"
#include "Weapon.h"
#include "../Ginger/Hades.h"

NetPlayerReceiver::NetPlayerReceiver()
{
}
NetPlayerReceiver::~NetPlayerReceiver()
{
}

void NetPlayerReceiver::EventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContentObj) {
	//�v���C���[�֌W�̃C�x���g���L���[�ɕۑ�
	if (eventCode > enPlayerEventStart && eventCode < enPlayerEventEnd) {
		ExitGames::Common::Hashtable eventContent = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable>(eventContentObj).getDataCopy();
		//�t���[����
		if (eventContent.getValue((nByte)enFrameCount)) {
			int frmcnt = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enFrameCount)))->getDataCopy();
			m_eventContentQueue[playerNr].push(std::make_tuple((NetworkEventCode)eventCode, frmcnt, eventContent));
		}
	}
}
	
void NetPlayerReceiver::RunEvent(int playerNr, bool frameSkip){

	bool isFrameSkipOn = false;

	while(!m_eventContentQueue[playerNr].empty()){
	
	//�������Ƃ肾��
	int frmcnt = std::get<1>(m_eventContentQueue[playerNr].front());

	//�t���[���X�L�b�v
	if (frameSkip) {
		const int FRAMESKIP_LAG = 6; // �t���[���ȏ�̃��O�̓X�L�b�v
		if (isFrameSkipOn || m_status[playerNr].m_cnt + FRAMESKIP_LAG <= frmcnt) {
			m_status[playerNr].m_cnt = frmcnt;
			isFrameSkipOn = true;
		}
	}
	
	//�܂����s���Ȃ�
	if (!m_status[playerNr].m_isNoReceive && frmcnt > m_status[playerNr].m_cnt) { return; }	

	//����C�x���g
	if (m_status[playerNr].m_isNoReceive) {
		m_status[playerNr].m_isNoReceive = false;
		m_status[playerNr].m_cnt = frmcnt;
	}

	NetworkEventCode eventCode = std::get<0>(m_eventContentQueue[playerNr].front());
	ExitGames::Common::Hashtable eventContent = std::get<2>(m_eventContentQueue[playerNr].front());
	m_eventContentQueue[playerNr].pop();

	switch (eventCode){

	case enNormal:
	{
		//�p�b�h����
		if (eventContent.getValue((nByte)enActionSender)) {
			//�{�^�����͂̃r�b�g�t�B�[���h���擾
			nByte buttons = ((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 5))))->getDataCopy();
			//�A�N�V�����Z���_�[���Z�b�g
			m_status[playerNr].m_actionSender = ActionSender(
				{
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 0))))->getDataCopy() / 100.0f - 1.0f,
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 1))))->getDataCopy() / 100.0f - 1.0f
				},
				(buttons & 0b1) != 0,
				(buttons & 0b10) != 0,
				{
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 2))))->getDataCopy() / 100.0f - 1.0f,
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 3))))->getDataCopy() / 100.0f - 1.0f,
					(float)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enActionSender + 4))))->getDataCopy() / 100.0f - 1.0f
				},
				(buttons & 0b100) != 0,
				(buttons & 0b1000) != 0,
				false,//(buttons & 0b100000) != 0,
				false//(buttons & 0b1000000) != 0
			);
			//��s���̃t���O
			m_status[playerNr].m_isUpd8Fly = true;
			m_status[playerNr].m_isFly = (buttons & 0b10000) != 0;			
		}

		//���b�N�I��
		if (eventContent.getValue((nByte)enLockOn)) {
			m_status[playerNr].m_isUpd8Lock = true;
			m_status[playerNr].m_lock = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enLockOn)))->getDataCopy();
		}

		//��������
		if (eventContent.getValue((nByte)enActiveWepon)) {
			m_status[playerNr].m_isUpd8ActiveWeapon = true;
			m_status[playerNr].m_activeWeapon = ((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)enActiveWepon)))->getDataCopy();
		}

		//�t���C���
		if (eventContent.getValue((nByte)enFlyTimer)) {
			m_status[playerNr].m_isUpd8FlyTimer = true;
			m_status[playerNr].m_flyTimer = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enFlyTimer)))->getDataCopy();
		}

		//�e��
		if (eventContent.getValue((nByte)enBulletCnt)) {
			m_status[playerNr].m_isUpd8BulletCnt = true;
			for (int i = 0; i < CPlayer::WEAPON_NUM; i++)
			{
				m_status[playerNr].m_bulletCnt[i] = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enBulletCnt + i))))->getDataCopy();
			}
		}

		//���W
		if (eventContent.getValue((nByte)enPosition)) {
			m_status[playerNr].m_isUpdatePos = true;
			m_status[playerNr].m_pos.Set(
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enPosition + 0))))->getDataCopy(),
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enPosition + 1))))->getDataCopy(),
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enPosition + 2))))->getDataCopy()
			);
		}
		//�x���V�e�B
		if (eventContent.getValue((nByte)enVelocity)) {
			m_status[playerNr].m_isUpd8Velocity = true;
			m_status[playerNr].m_velocity.Set(
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enVelocity + 0))))->getDataCopy(),
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enVelocity + 1))))->getDataCopy(),
				(float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enVelocity + 2))))->getDataCopy()
			);
		}

		//�s���ʒu
		int offset = 0;
		if (eventContent.getValue((nByte)enZombiePosAvg)) {
			int num = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enZombiePosAvg)))->getDataCopy();
			for (int i = 0; i < num; i++) {
				//id
				int id = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				//���W
				CVector3 pos;
				pos.x = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				pos.y = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				pos.z = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				m_citizenPosListAvg.emplace_back(std::make_pair(id,pos));
			}
		}
		if (eventContent.getValue((nByte)enZombiePosSync)) {
			int num = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enZombiePosSync)))->getDataCopy();
			for (int i = 0; i < num; i++) {
				//id
				int id = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				//����
				int time = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				//���W
				CVector3 pos;
				pos.x = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				pos.y = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				pos.z = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				m_citizenPosListSync.emplace_back(std::make_tuple(id, pos, playerNr, time));
			}
		}
		//�s��Mover
		if (eventContent.getValue((nByte)enZombieMover)) {
			int num = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enZombieMover)))->getDataCopy();
			for (int i = 0; i < num; i++) {
				//id
				int id = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				//����
				int time = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				//���W
				CVector3 pos;
				pos.x = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				pos.y = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				pos.z = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				//MoverVec
				CVector3 MoverVec;
				MoverVec.x = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				MoverVec.y = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				MoverVec.z = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(((int)enZombiePos + offset))))->getDataCopy(); offset++;
				m_citizenMoverSyncList.emplace_back(std::make_tuple(id, pos, playerNr, time, MoverVec));
			}
		}
	}
	break;

	case enReliable:
	{
		//�ő���
		if (eventContent.getValue((nByte)enKenzoku)) {
			int num = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enKenzoku)))->getDataCopy();
			int i2 = enKenzoku + 1;
			for (int i = 0; i < num; i++) {
				int id = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(i2)))->getDataCopy(); i2++;//ID
				int time = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(i2)))->getDataCopy(); i2++;//����
				//���W
				CVector3 pos;
				pos.x = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(i2)))->getDataCopy(); i2++;
				pos.y = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(i2)))->getDataCopy(); i2++;
				pos.z = (float)((ExitGames::Common::ValueObject<int>*)(eventContent.getValue(i2)))->getDataCopy(); i2++;

				auto C = m_citizensStatus.find(id);
				if (C == m_citizensStatus.end()) {
					//�V�K�쐬
					m_citizensStatus.emplace(id, CitizensStatus() = { time, playerNr, pos });
				}
				else {
					//���Ԃ��V���� or ���Ԃ������Ńv���C���[�ԍ����傫��
					if (C->second.timeCnt < time || C->second.timeCnt == time && C->second.plyNum < playerNr) {
						//�㏑��
						C->second = { time, playerNr, pos };
					}
				}
			}
		}

		//�_�̗͊l��
		if (eventContent.getValue((nByte)enGetGodpower)) {
			int gingerN = (int)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enGetGodpower+0))))->getDataCopy();
			int plyN = (int)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)(enGetGodpower+1))))->getDataCopy();
			GetGodPower(gingerN, plyN);
		}
		//�_�̗͎g�p
		if (eventContent.getValue((nByte)enUseGodPower)) {
			GodPowerType type = (GodPowerType)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)enUseGodPower)))->getDataCopy();
			UseGodPower(type, playerNr);
		}

		//���V���X�̏���
		if (eventContent.getValue((nByte)enSummonWosiris)) {
			//����
			if (m_gingerGene) {
				int rot = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enSummonWosiris + 0))))->getDataCopy();
				new Wosiris(m_pCPlayer[playerNr], CMath::DegToRad((float)rot), m_gingerGene);
			}
			//����
			if (m_citizenGene) {
				int citizen[3];
				citizen[0] = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enSummonWosiris + 1))))->getDataCopy();
				citizen[1] = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enSummonWosiris + 2))))->getDataCopy();
				citizen[2] = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)(enSummonWosiris + 3))))->getDataCopy();
				for (int i = 0; i < 3; i++) {
					if (citizen[i] >= 0) {
						m_citizenGene->GetCitizen(citizen[i])->Death();
					}
				}
			}
		}
		//���V���X�̃R���g���[���D��
		if (eventContent.getValue((nByte)enGetControlWosiris)) {
			if (m_gingerGene && m_gingerGene->GetWosiris()) {
				Wosiris* pWosiris = m_gingerGene->GetWosiris();
				int time = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)enGetControlWosiris)))->getDataCopy();
				//���Ԃ��V���� or ���Ԃ������Ńv���C���[�ԍ����傫��
				if (pWosiris->GetLastControlTime() < time || pWosiris->GetLastControlTime() == time && playerNr > pWosiris->GetLastControlPly()) {
					//�X�V
					if (m_pCPlayer[playerNr]) {
						pWosiris->SetLastControl(playerNr, time);
						pWosiris->ChangeControl(m_pCPlayer[playerNr]);
					}
				}
			}
		}
	}
	break;

	case enKillPlayer:
	{
		//�E���ꂽ
		m_status[GetPhoton()->GetLocalPlayerNumber()].m_isDead = true;
		m_status[GetPhoton()->GetLocalPlayerNumber()].m_isUpdateDead = true;
	}
	break;

	case enPlayerDeadOrAlive:
	{
		//��������
		m_status[playerNr].m_isDead = ((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)1)))->getDataCopy();
		m_status[playerNr].m_isUpdateDead = true;
	}
	break;

	case enDestroyGinger:
	{
		//�_�Дj��
		int offset = 1;
		int num = (int)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)offset)))->getDataCopy(); offset++;
		for (int i = 0; i < num; i++) {
			int time = ((ExitGames::Common::ValueObject<int>*)(eventContent.getValue((nByte)offset)))->getDataCopy(); offset++;
			int id = (int)((ExitGames::Common::ValueObject<nByte>*)(eventContent.getValue((nByte)offset)))->getDataCopy(); offset++;
			AddGodPowerLottery(id, time, playerNr);
		}
	}
	break;

	default:
		break;
	}

	}
}

void NetPlayerReceiver::PreUpdate() {
	for (int i = 0; i < NET_MAX_PLAYER + 1; i++) {
		//�C�x���g���s
		RunEvent(i);

		//�v���C���[�ɏ��n��
		UpdatePlayer(i);
		//�s���ɏ��n��
		UpdateCitizen();

		UpdateGodPower();

		//�J�E���^�[�i�߂�
		if (!m_status[i].m_isNoReceive) { m_status[i].m_cnt++; }
	}
}

void NetPlayerReceiver::PostLoopUpdate() {
	for (int i = 0; i < NET_MAX_PLAYER + 1; i++) {
		//�C�x���g���s
		RunEvent(i,true);

		//�v���C���[�ɏ��n��
		UpdatePlayer(i);
		//�s���ɏ��n��
		UpdateCitizen();

		UpdateGodPower();
	}
}

//�v���C���[�ɏ��n��
void NetPlayerReceiver::UpdatePlayer(int playerNr) {
	if (m_pCPlayer[playerNr] && m_pCPlayer[playerNr]->GetIsInit()) {
		
		if (playerNr != GetPhoton()->GetLocalPlayerNumber()) {//�����͏���
			//�A�N�V����
			m_pCPlayer[playerNr]->sendAction(m_status[playerNr].m_actionSender);
			
			//���b�N�I��
			if (m_status[playerNr].m_isUpd8Lock) {
				bool isPly = m_pCPlayer[playerNr]->GetLockOnIsPly();
				int lock = m_status[playerNr].m_lock;
				
				if (lock == 0) {
					//�m�[���b�N
					lock = -1;
				}
				else {
					if (lock < 0) { 
						//�v���C���[
						isPly = true;
						lock *= -1;
					}
					else {
						//�s��
						isPly = false;
					}
					lock--;
				}

				m_pCPlayer[playerNr]->SetLockOn(isPly, lock);
				m_status[playerNr].m_isUpd8Lock = false;
			}	

			//��������
			if (m_status[playerNr].m_isUpd8ActiveWeapon) {
				m_pCPlayer[playerNr]->changeWeapon(m_status[playerNr].m_activeWeapon);
				m_status[playerNr].m_isUpd8ActiveWeapon = false;
			}
			//�t���C���
			if (m_status[playerNr].m_isUpd8FlyTimer) {
				m_pCPlayer[playerNr]->SetFlyTimer(m_status[playerNr].m_flyTimer);
				m_status[playerNr].m_isUpd8FlyTimer = false;
			}
			//��s�t���O
			if (m_status[playerNr].m_isUpd8Fly) {
				if (m_status[playerNr].m_isFly) {
					if (!m_pCPlayer[playerNr]->isFlying()) { m_pCPlayer[playerNr]->fly(); }
				}
				else {
					if (m_pCPlayer[playerNr]->isFlying()) { m_pCPlayer[playerNr]->flyStop(); }
				}
				m_status[playerNr].m_isUpd8Fly = false;
			}
			//�e��
			if (m_status[playerNr].m_isUpd8BulletCnt) {
				Weapon** w = m_pCPlayer[playerNr]->GetWeapons();
				for (int i = 0; i < CPlayer::WEAPON_NUM; i++)
				{
					w[i]->setBulletCount(m_status[playerNr].m_bulletCnt[i]);
				}
				m_status[playerNr].m_isUpd8BulletCnt = false;
			}
			//���W
			if (m_status[playerNr].m_isUpdatePos) {
				m_pCPlayer[playerNr]->SetPosition(m_status[playerNr].m_pos);
				m_status[playerNr].m_isUpdatePos = false;
			}
			//�x���V�e�B
			if (m_status[playerNr].m_isUpd8Velocity) {
				m_pCPlayer[playerNr]->SetVelocity(m_status[playerNr].m_velocity);
				m_status[playerNr].m_isUpd8Velocity = false;
			}
		}

		//��������
		if (m_status[playerNr].m_isUpdateDead) {
			if (m_status[playerNr].m_isDead) {
				m_pCPlayer[playerNr]->Death();
			}
			else {
				m_pCPlayer[playerNr]->Revive();
			}
			//�đ���h���ׂ��L���X�^������
			if (playerNr != GetPhoton()->GetLocalPlayerNumber()) {
				m_pCaster[playerNr]->SetIsDead(m_status[playerNr].m_isDead);
			}
			m_status[playerNr].m_isUpdateDead = false;
		}

	}
}

//�s���ɏ��n��
void NetPlayerReceiver::UpdateCitizen() {
	if (m_citizenGene) {
		//�ő���
		for (auto& cs : m_citizensStatus) {
			auto citizen = m_citizenGene->GetCitizen(cs.first);
			if (!citizen) { continue; }
			//���Ԃ��V���� or ���Ԃ������Ńv���C���[�ԍ����傫��
			if (citizen->GetLastKenzokuingCnt() < cs.second.timeCnt || citizen->GetLastKenzokuingCnt() == cs.second.timeCnt && cs.second.plyNum > citizen->GetLastKenzokuingPly()) {
				//�X�V(�s���̏����E���W)�E�ő���
				if (m_pCPlayer[cs.second.plyNum]) {
					citizen->SetLastKenzokuingCnt(cs.second.timeCnt);
					citizen->SetLastKenzokuingPly(cs.second.plyNum);
					citizen->setPos(cs.second.pos);
					citizen->ChangeToKenzoku(m_pCPlayer[cs.second.plyNum]);
				}
			}
		}
		m_citizensStatus.clear();

		//�ʒu����
		for (auto& cs : m_citizenPosListAvg) {
			auto citizen = m_citizenGene->GetCitizen(cs.first);
			if (!citizen) { continue; }
			//�X�V
			//citizen->SetIsSend(false);
			citizen->setPos((citizen->getPos() + cs.second)*0.5f);
		}
		m_citizenPosListAvg.clear();

		//�ʒu����
		for (auto& cs : m_citizenPosListSync) {
			auto citizen = m_citizenGene->GetCitizen(std::get<0>(cs));
			if (!citizen) { continue; }
			//���Ԃ��V���� or ���Ԃ������Ńv���C���[�ԍ����傫����������
			if (citizen->GetTargetCnt() < std::get<3>(cs) || citizen->GetTargetCnt() == std::get<3>(cs) && std::get<2>(cs) >= citizen->GetTargetPly()) {
				//�X�V
				citizen->SetIsSend(false);
				citizen->setPos(std::get<1>(cs));
				citizen->SetTargetPly(std::get<2>(cs));
				citizen->SetTargetCnt(std::get<3>(cs));
			}
		}
		m_citizenPosListSync.clear();

		//Mover����
		for (auto& cs : m_citizenMoverSyncList) {
			auto citizen = m_citizenGene->GetCitizen(std::get<0>(cs));
			if (!citizen) { continue; }
			//���Ԃ��V���� or ���Ԃ������Ńv���C���[�ԍ����傫����������
			if (citizen->GetSyncMoverCnt() < std::get<3>(cs) || citizen->GetSyncMoverCnt() == std::get<3>(cs) && std::get<2>(cs) >= citizen->GetSyncMoverPly()) {
				//�X�V
				citizen->SetIsSendMover(false);
				citizen->setPos(std::get<1>(cs));
				citizen->SetSyncMoverPly(std::get<2>(cs));
				citizen->SetSyncMoverCnt(std::get<3>(cs));
				citizen->SetMoverNetVec(std::get<4>(cs));
			}
		}
		m_citizenMoverSyncList.clear();		
	}
}

void NetPlayerReceiver::UpdateGodPower() {
	//�_�̗͂�����
	for (auto& T : m_godPowerLotteryTimer) {
		if (T.second > 0) {
			T.second--;
			if (T.second <= 0) {
				//�l��
				GetGodPower(T.first, m_godPowerLotteryList[T.first].second);

				if (m_pCaster[GetPhoton()->GetLocalPlayerNumber()]) {
					//�݂�Ȃɓ`����
					m_pCaster[GetPhoton()->GetLocalPlayerNumber()]->SendGetGodPower(T.first, m_godPowerLotteryList[T.first].second);
				}
				m_godPowerLotteryTimer.erase(T.first);
			}
		}
	}
}
void NetPlayerReceiver::AddGodPowerLottery(int id, int time, int plyNum) {

	if (!GetPhoton()->GetIsMasterClient()) { return; }

	if (m_godPowerLotteryList.count(id) == 0) {
		//�V�K
		m_godPowerLotteryList.emplace(id, std::make_pair(time, plyNum));
		
		if (m_godPowerLotteryTimer.count(id) == 0) {
			//�^�C�}�[�J�n
			m_godPowerLotteryTimer.emplace(id, 30);
		}
	}
	else {
		//���Ԃ��Â��ƗD��
		if (m_godPowerLotteryList[id].first > time) {
			m_godPowerLotteryList.insert_or_assign(id, std::make_pair(time, plyNum));
		}
	}
}

void NetPlayerReceiver::GetGodPower(int jinjyaNum,int plyNum) {
	if (!m_gingerGene) { return; }
	
	//�l��
	if (m_pCPlayer[plyNum]) {
		m_pCPlayer[plyNum]->SetGodPower(m_gingerGene->GetGinger(jinjyaNum)->GetPowerType());
	}
	//�_�Дj��
	m_gingerGene->GetGinger(jinjyaNum)->Destory();
}
void NetPlayerReceiver::UseGodPower(GodPowerType type, int plyNum) {
	if (!m_gingerGene || !m_pCPlayer[plyNum]) { return; }
	
	switch (type) {
	case enHades:
		new Hades(m_pCPlayer[plyNum], m_gingerGene);
		break;
	default:
		break;
	}
}