#pragma once

class InstancingCitizenColorManager : public IQSGameObject
{
public:
	InstancingCitizenColorManager(GameObj::InstancingModel* insModel) {
		Init(insModel);
	}
	~InstancingCitizenColorManager() {
		Release();
	}

	void Init(GameObj::InstancingModel* insModel);
	void Release();

	void AddColor(const CVector4& color);

	void PostLoopPostUpdate()override;

private:
	int m_instanceNum = 0, m_instanceMax = 0;
	std::unique_ptr<CVector4[]>	m_color;
	ID3D11Buffer*				m_colorSB = nullptr;
	ID3D11ShaderResourceView*	m_colorSRV = nullptr;

	Shader m_psShader;

	GameObj::InstancingModel* m_insModel = nullptr;
};

