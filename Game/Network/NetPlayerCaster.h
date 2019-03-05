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

	bool m_isDead = false;

	std::list<std::pair<int, int>> m_sendKenzokuList;
};

