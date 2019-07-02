#pragma once

class InstancingCitizenColorManager : public GameObj::InstancingModel::IInstancesData {
private:
	//�Ċm��
	void Reset(int insMaxNum);

public:
	//�R���X�g���N�^
	InstancingCitizenColorManager(GameObj::InstancingModel* insModel);
	//�f�X�g���N�^
	~InstancingCitizenColorManager();

	void PreDraw(int instanceNum, int drawInstanceNum, const std::unique_ptr<bool[]>& drawInstanceMask)override;
	void AddDrawInstance(int instanceIndex, const CMatrix& SRTMatrix, const CVector3& scale, void *param)override;
	void SetInstanceMax(int instanceMax)override;
	
private:
	int m_instanceMax = 0;

	Shader m_psShader;
	GameObj::InstancingModel* m_insModel = nullptr;

	//�s���F
	std::unique_ptr<CVector4[]>							m_color, m_colorCache;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				m_colorSB;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_colorSRV;
};

