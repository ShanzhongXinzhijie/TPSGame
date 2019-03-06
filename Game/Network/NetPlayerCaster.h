#pragma once

class CPlayer;
class Citizen;

class NetPlayerCaster : public IQSGameObject
{
public:
	NetPlayerCaster(CPlayer* pCPlayer);
	~NetPlayerCaster();

	void PostUpdate() override;

	void SetIsDead(bool isdead) { m_isDead = isdead; }

	void SendNewKenzoku(::Citizen* pkenzoku);

private:
	CPlayer* m_pCPlayer = nullptr;
	int m_cnt = INT_MIN;

	bool m_isReload = false;

	int m_coolDowmSendBulletCnt = 0;

	bool m_isDead = false;

	std::list<std::tuple<int, int, CVector3>> m_sendKenzokuList;
};

