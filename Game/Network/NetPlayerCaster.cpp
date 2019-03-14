#include "stdafx.h"
#include "NetPlayerCaster.h"
#include "CPlayer.h"
#include "Citizen.h"
#include "Network.h"
#include "Weapon.h"
#include "CitizenGene.h"

NetPlayerCaster::NetPlayerCaster(CPlayer* pCPlayer, CitizenGene* citizenGene)
{
	m_pCPlayer = pCPlayer;
	m_pCPlayer->SetNetCaster(this);
	m_isDead = m_pCPlayer->GetIsDead();

	m_citizenGene = citizenGene;
}

NetPlayerCaster::~NetPlayerCaster()
{
}

void NetPlayerCaster::PostUpdate() {
	if (GetPhoton()->GetState() != PhotonNetworkLogic::JOINED) { return; }
	
	//�������g��?
	bool isMe = false;
	if (m_pCPlayer->playerNum == GetPhoton()->GetLocalPlayerNumber()) {
		isMe = true;
	}	

	//�������g�Ȃ���s
	if (isMe) {
		const ActionSender& AS = m_pCPlayer->GetActionSender();

		//�����[�h�̓��͂��L�^(���͂�1f����true�ɂȂ�Ȃ����߁A4f�Ɉ��̑��M�^�C�~���O�ł̎擾�ł͔������ł���)
		if (!m_isReload) { m_isReload = AS.isReload(); }

		//�N�[���_�E�����Ԑi�߂�
		m_coolDowmSendFlyTimer = max(m_coolDowmSendFlyTimer - 1, 0);
		m_coolDowmSendBulletCnt = max(m_coolDowmSendBulletCnt - 1, 0);

		//�m���ȑ��M
		{
			ExitGames::Common::Hashtable _event;
			//�t���[���ԍ�
			_event.put((nByte)enFrameCount, m_cnt);

			bool isSend = false;

			//12F�Ɉ�񑗐M
			if (m_cnt % 12 == 0) {
				//�ő����𑗐M
				if (m_sendKenzokuList.size() > 0) {
					_event.put((nByte)1, (int)m_sendKenzokuList.size());//����
					int i = 2;
					for (auto& K : m_sendKenzokuList) {
						_event.put(i, (int)std::get<0>(K)->GetUniqueID()); i++;//ID
						_event.put(i, std::get<1>(K)); i++;//����
						//���W
						const CVector3& pos = std::get<0>(K)->getPos();
						_event.put(i, (int)pos.x); i++;
						_event.put(i, (int)pos.y); i++;
						_event.put(i, (int)pos.z); i++;
						//����͈Ȍ㑗�M���Ȃ�
						//m_citizenGene->GetCitizen(std::get<0>(K))->SetIsSend(false);
					}
					m_sendKenzokuList.clear();

					isSend = true;
				}
			}

			//�m���ȑ��M
			if (isSend) {
				GetPhoton()->Send(enKenzoku, _event, true);
			}
		}

		//�m���łȂ����M
		{
			ExitGames::Common::Hashtable _event;
			//�t���[���ԍ�
			_event.put((nByte)enFrameCount, m_cnt);

			bool isSend = false;

			//4F�Ɉ�񑗐M
			if (m_cnt % 4 == 0) {
				//�A�i���O����
				_event.put((nByte)(enActionSender + 0), (nByte)(std::round(AS.getMovement().x*100.0f) + 100));
				_event.put((nByte)(enActionSender + 1), (nByte)(std::round(AS.getMovement().y*100.0f) + 100));
				_event.put((nByte)(enActionSender + 2), (nByte)(std::round(AS.getLookVec().x*100.0f) + 100));
				_event.put((nByte)(enActionSender + 3), (nByte)(std::round(AS.getLookVec().y*100.0f) + 100));
				_event.put((nByte)(enActionSender + 4), (nByte)(std::round(AS.getLookVec().z*100.0f) + 100));
				//�{�^�����́E�t���O
				nByte bottuns = 0;
				if (AS.isJump()) { bottuns = bottuns | 0b1; }
				if (AS.isDash()) { bottuns = bottuns | 0b10; }
				if (AS.isShot()) { bottuns = bottuns | 0b100; }
				if (m_isReload) { bottuns = bottuns | 0b1000; } m_isReload = false;
				if (m_pCPlayer->isFlying()) { bottuns = bottuns | 0b10000; }
				//if (AS.isWeaponLeft()) { bottuns = bottuns | 0b100000; }
				//if (AS.isWeaponRight()) { bottuns = bottuns | 0b1000000; }
				_event.put((nByte)(enActionSender + 5), (nByte)bottuns);

				//��������
				_event.put((nByte)enActiveWepon, (nByte)m_pCPlayer->GetActiveWeapon());

				//�t���C���
				if (m_pCPlayer->isFlying() && m_coolDowmSendFlyTimer <= 0) {
					_event.put((nByte)enFlyTimer, (int)m_pCPlayer->getFlyTimer());
					m_coolDowmSendFlyTimer = 60;
				}

				//�e��
				if (AS.isShot() && m_coolDowmSendBulletCnt <= 0) {//�ˌ�����Ȃ瑗��
					const Weapon* const * weapons = m_pCPlayer->GetWeapons();
					for (int i = 0; i < CPlayer::WEAPON_NUM; i++)
					{
						_event.put((nByte)(enBulletCnt + i), weapons[i]->getBulletCount());
					}
					m_coolDowmSendBulletCnt = 60;
				}

				//���W
				_event.put((nByte)(enPosition + 0), (int)std::round(m_pCPlayer->getPosition().x));
				_event.put((nByte)(enPosition + 1), (int)std::round(m_pCPlayer->getPosition().y));
				_event.put((nByte)(enPosition + 2), (int)std::round(m_pCPlayer->getPosition().z));

				//�x���V�e�B 
				_event.put((nByte)(enVelocity + 0), (int)std::round(m_pCPlayer->getVelocity().x));
				_event.put((nByte)(enVelocity + 1), (int)std::round(m_pCPlayer->getVelocity().y));
				_event.put((nByte)(enVelocity + 2), (int)std::round(m_pCPlayer->getVelocity().z));

				isSend = true;
			}

			//36F�Ɉ�񑗐M
			if (m_cnt % 36 == 0) {
				//�s���E�]���r�̈ʒu
				int citiNum = (int)m_citizenGene->GetCitizenNum();
				int offset = 0;

				//�����_�����M
				/*if (m_sendCitiInd < 0) {
					m_sendCitiInd = CMath::RandomInt() % (int)m_citizenGene->GetCitizenNum();
				}
				for (int i = m_sendCitiInd; i < min(citiNum, m_sendCitiInd + 4); i++) {
					SendSyncCitizen(m_citizenGene->GetCitizen(i));
				}
				m_sendCitiInd += 4;
				if (m_sendCitiInd >= citiNum) {
					m_sendCitiInd -= citiNum;
					if (m_sendCitiInd >= citiNum) {
						m_sendCitiInd = -1;
					}
				}*/
				
				//�ʒu����
				if (m_sendAvgList.size() > 0) {
					int avgNum = 0;
					for (auto& K : m_sendAvgList) {
						if (K->GetIsAvg()) {
							_event.put(((int)enZombiePos + offset), (int)K->GetUniqueID()); offset++;
							_event.put(((int)enZombiePos + offset), (int)std::round(K->getPos().x)); offset++;
							_event.put(((int)enZombiePos + offset), (int)std::round(K->getPos().y)); offset++;
							_event.put(((int)enZombiePos + offset), (int)std::round(K->getPos().z)); offset++;
							avgNum++;
							//��������Ȃ�
							K->SetIsSend(false);
						}
					}
					m_sendAvgList.clear();

					if (avgNum > 0) {
						//��
						_event.put((nByte)enZombiePosAvg, avgNum);
						isSend = true;
					}
				}
				//�ʒu����
				if (m_sendSyncList.size() > 0) {
					int syncNum = 0;
					for (auto& K : m_sendSyncList) {
						_event.put(((int)enZombiePos + offset), (int)K->GetUniqueID()); offset++;
						_event.put(((int)enZombiePos + offset), K->GetNetCnt()); offset++;
						_event.put(((int)enZombiePos + offset), (int)std::round(K->getPos().x)); offset++;
						_event.put(((int)enZombiePos + offset), (int)std::round(K->getPos().y)); offset++;
						_event.put(((int)enZombiePos + offset), (int)std::round(K->getPos().z)); offset++;
						syncNum++;
						//��������Ȃ�
						K->SetIsSend(false);
						K->SetTargetPly(m_pCPlayer->playerNum);
						K->SetTargetCnt(K->GetNetCnt());
					}
					m_sendSyncList.clear();

					if (syncNum > 0) {
						//��
						_event.put((nByte)enZombiePosSync, syncNum);
						isSend = true;
					}
				}
				//Mover����
				if (m_sendMoverList.size() > 0) {
					int Num = 0;
					for (auto& K : m_sendMoverList) {
						_event.put(((int)enZombiePos + offset), (int)K->GetUniqueID()); offset++;
						_event.put(((int)enZombiePos + offset), K->GetNetCnt()); offset++;
						_event.put(((int)enZombiePos + offset), (int)std::round(K->getPos().x)); offset++;
						_event.put(((int)enZombiePos + offset), (int)std::round(K->getPos().y)); offset++;
						_event.put(((int)enZombiePos + offset), (int)std::round(K->getPos().z)); offset++;
						CVector3 MoverVec = K->GetMoverNetVec();
						_event.put(((int)enZombiePos + offset), (int)std::round(MoverVec.x)); offset++;
						_event.put(((int)enZombiePos + offset), (int)std::round(MoverVec.y)); offset++;
						_event.put(((int)enZombiePos + offset), (int)std::round(MoverVec.z)); offset++;
						Num++;
						//��������Ȃ�
						K->SetIsSendMover(false);
						K->SetSyncMoverPly(m_pCPlayer->playerNum);
						K->SetSyncMoverCnt(K->GetNetCnt());
					}
					m_sendMoverList.clear();

					if (Num > 0) {
						//��
						_event.put((nByte)enZombieMover, Num);
						isSend = true;
					}
				}
			}

			//���M
			if (isSend) {
				GetPhoton()->Send(enNormal, _event);
			}
		}
	}

	//����ł鐶���Ă�
	if(m_isDead != m_pCPlayer->GetIsDead()){
		m_isDead = m_pCPlayer->GetIsDead();

		ExitGames::Common::Hashtable _event;
		_event.put((nByte)enFrameCount, m_cnt);//�t���[���ԍ�

		//�E�����I
		if (!isMe) {
			if (m_isDead) {
				//�E��������ɑ���
				GetPhoton()->Send(enKillPlayer, _event, true, ExitGames::LoadBalancing::RaiseEventOptions().setTargetPlayers(&m_pCPlayer->playerNum, 1));
			}
		}
		else {
			//���񂾁E�����Ԃ���
			_event.put((nByte)1, (nByte)m_isDead);
			GetPhoton()->Send(enPlayerDeadOrAlive, _event, true);
			
			//�L���b�V������
			//int num = GetPhoton()->GetLocalPlayerNumber();
			//GetPhoton()->Send(enPlayerDeadOrAlive, ExitGames::Common::Hashtable(), true, ExitGames::LoadBalancing::RaiseEventOptions().setEventCaching(ExitGames::Lite::EventCache::REMOVE_FROM_ROOM_CACHE).setTargetPlayers(&num, 1));
			//GetPhoton()->Send(enPlayerDeadOrAlive, _event, true, ExitGames::LoadBalancing::RaiseEventOptions().setEventCaching(ExitGames::Lite::EventCache::ADD_TO_ROOM_CACHE));
		}
	}

	m_cnt++;
}

void NetPlayerCaster::SendNewKenzoku(::Citizen* pkenzoku) {
	m_sendKenzokuList.emplace_back(std::make_pair(pkenzoku, pkenzoku->GetNetCnt()));
	pkenzoku->SetLastKenzokuingCnt(pkenzoku->GetNetCnt());
	pkenzoku->SetLastKenzokuingPly(GetPhoton()->GetLocalPlayerNumber());
}

void NetPlayerCaster::SendMover(::Citizen* pcitizen) {
	if (!pcitizen->GetIsSendMover()) {
		m_sendMoverList.emplace_back(pcitizen);
	}
	pcitizen->SetIsSendMover(true);
	pcitizen->SetSyncMoverCnt(pcitizen->GetNetCnt());
	pcitizen->SetSyncMoverPly(GetPhoton()->GetLocalPlayerNumber());
}

void NetPlayerCaster::SendAvgCitizen(::Citizen* pcitizen) {
	if (!pcitizen->GetIsSend() && !pcitizen->GetIsSendMover()) {
		m_sendAvgList.emplace_back(pcitizen);
		
		pcitizen->SetIsSend(true);
		pcitizen->SetIsAvg(true);
	}
}

void NetPlayerCaster::SendSyncCitizen(::Citizen* pcitizen) {
	if (!pcitizen->GetIsSend()) {
		m_sendSyncList.emplace_back(pcitizen);
	}
	pcitizen->SetIsSend(true);
	pcitizen->SetIsAvg(false);
	pcitizen->SetTargetPly(m_pCPlayer->playerNum);
	pcitizen->SetTargetCnt(pcitizen->GetNetCnt());
}
