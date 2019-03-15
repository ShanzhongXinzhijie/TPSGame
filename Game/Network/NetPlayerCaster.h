#pragma once

#include"CPlayer.h"
class Citizen;
class CitizenGene;

class NetPlayerCaster : public IQSGameObject
{
public:
	NetPlayerCaster(CPlayer* pCPlayer, CitizenGene* citizenGene);
	~NetPlayerCaster();

	void PostUpdate() override;

	void SetIsDead(bool isdead) { m_isDead = isdead; }

	void SendNewKenzoku(::Citizen* pkenzoku);
	void SendMover(::Citizen* pkenzoku);
	void SendAvgCitizen(::Citizen* pcitizen);
	void SendSyncCitizen(::Citizen* pcitizen);

private:
	CPlayer* m_pCPlayer = nullptr;
	int m_cnt = INT_MIN;

	bool m_isReload = false;
	bool m_isShot   = false;
	int m_bullet[CPlayer::WEAPON_NUM] = {};

	int m_coolDowmSendFlyTimer = 0;
	int m_coolDowmSendBulletCnt = 0;

	bool m_isDead = false;

	//int m_sendCitiInd = -1;
	CitizenGene* m_citizenGene = nullptr;
	std::list<std::pair<::Citizen*, int>> m_sendKenzokuList;
	std::list<::Citizen*> m_sendAvgList, m_sendSyncList, m_sendMoverList;
};

