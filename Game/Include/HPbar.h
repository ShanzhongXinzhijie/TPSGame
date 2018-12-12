#pragma once
class HPbar {
public:
	HPbar();
	~HPbar();

private:
	static constexpr unsigned int c_hp = 50;
	unsigned int m_hp;
	CSprite m_inSpr;
	CSprite m_outSpr;

	CVector2 m_pos;
};

