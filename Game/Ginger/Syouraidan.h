#pragma once

class CPlayer;

class Syouraidan : public IGameObject
{
public:
	Syouraidan(const CVector3& pos, CPlayer* target);
	~Syouraidan();

	void Update()override;

private:
	SuicideObj::CEffekseer* m_eff = nullptr; 
	float m_effCnt = 0.0f;
	CVector3 m_pos;
	CPlayer* m_target;
	float m_time = 0.0f;
};

