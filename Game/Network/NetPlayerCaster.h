#pragma once

class CPlayer;
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
	void SendAvgCitizen(::Citizen* pcitizen);
	void SendSyncCitizen(::Citizen* pcitizen);

private:
	CPlayer* m_pCPlayer = nullptr;
	int m_cnt = INT_MIN;

	bool m_isReload = false;

	int m_coolDowmSendFlyTimer = 0;
	int m_coolDowmSendBulletCnt = 0;

	bool m_isDead = false;

	CitizenGene* m_citizenGene = nullptr;
	std::list<std::pair<int, int>> m_sendKenzokuList;
};

