#pragma once
class Flybar{
public:
	Flybar();
	~Flybar();

	void Draw(float flypower, float maxflypower);

	void setPosition(const CVector2& pos) {
		m_pos = pos;
	};

private:
	CSprite m_inSpr;
	CSprite m_outSpr;

	CVector2 m_pos = { 1270.0f,10.0f };
};

