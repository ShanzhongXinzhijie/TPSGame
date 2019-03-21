#pragma once
class HPbar {
public:
	HPbar(float maxHp, float maxFly);
	~HPbar();

	void Draw(float hp, float fly, bool flyRest);

	void setPosition(const CVector2& pos) {
		m_pos = pos;
	};

private:
	CSprite m_inSpr;
	CSprite m_inFly;
	CSprite m_outSpr;

	const float maxHp;
	const float maxFly;

	CVector2 m_pos = {1270.0f,10.0f};

	float bright = 0.0f;
	float recoverWhite = 0.0f;
	char brightMove = 2;
};

