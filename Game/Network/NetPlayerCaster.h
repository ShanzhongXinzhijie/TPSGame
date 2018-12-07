#pragma once

class CPlayer;

class NetPlayerCaster : public IQSGameObject
{
public:
	NetPlayerCaster(CPlayer* pCPlayer);
	~NetPlayerCaster();

	void PostUpdate() override;

private:
	CPlayer* m_pCPlayer = nullptr;
	unsigned long int m_cnt = 0;

	bool m_isDead = false;
};

