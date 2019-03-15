#pragma once
class MiniHPbar : public IGameObject {
public:
	MiniHPbar(float maxHp);
	~MiniHPbar();

	void Update() override;
	void PostRender() override;

	void setIsEnable(bool enable) {
		this->enable = enable;
	}

	void display(unsigned int nowHp) {
		if (enable) {
			m_hp = nowHp;
			drawTime = 1.5f;
		}
	}

	void setPos(const CVector3& pos) {
		if (enable) {
			m_pos = pos;
			m_pos.y += 130.0f;
		}
	}

private:
	CSprite outImg;
	CSprite inImg;
	CSprite backImg;

	bool enable = true;

	CVector3 m_pos = {};

	const float c_maxHp;
	unsigned int m_hp = (unsigned int)c_maxHp;

	float drawTime = 0.0f;
};

