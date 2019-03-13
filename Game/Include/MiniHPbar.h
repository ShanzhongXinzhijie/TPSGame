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

	void setPos(CVector3 pos) {
		if (enable) {
			pos.y += 130.0f;
			m_pos = GetMainCamera()->CalcScreenPosFromWorldPosScreenPos(pos);
			m_scale = (3000.0f - (GetMainCamera()->GetPos() - pos).Length())/2000.0f;
			if (m_scale < 0.0f) {
				m_scale = 0.0f;
			}
		}
	}

private:
	CSprite outImg;
	CSprite inImg;
	CSprite backImg;

	bool enable = true;

	CVector2 m_pos = {};
	float m_scale;

	const float c_maxHp;
	unsigned int m_hp = (unsigned int)c_maxHp;

	float drawTime = 0.0f;
};

