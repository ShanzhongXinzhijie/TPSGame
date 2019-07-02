#include "stdafx.h"
#include "InstancingCitizenColorManager.h"
#include "DemolisherWeapon\Graphic\Model\SkinModelShaderConst.h"

void InstancingCitizenColorManager::Reset(int insMaxNum) {
	//最大インスタンス数設定
	m_instanceMax = insMaxNum;
	
	//色配列の確保
	m_color = std::make_unique<CVector4[]>(m_instanceMax);
	m_colorCache = std::make_unique<CVector4[]>(m_instanceMax);

	//StructuredBufferの確保
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	int stride = sizeof(CVector4);
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = static_cast<UINT>(stride * m_instanceMax);
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = stride;
	GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&desc, NULL, m_colorSB.ReleaseAndGetAddressOf());

	//ShaderResourceViewの確保
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	descSRV.BufferEx.FirstElement = 0;
	descSRV.Format = DXGI_FORMAT_UNKNOWN;
	descSRV.BufferEx.NumElements = desc.ByteWidth / desc.StructureByteStride;
	GetGraphicsEngine().GetD3DDevice()->CreateShaderResourceView(m_colorSB.Get(), &descSRV, m_colorSRV.ReleaseAndGetAddressOf());
}

InstancingCitizenColorManager::InstancingCitizenColorManager(GameObj::InstancingModel* insModel) : m_insModel(insModel){	
	//リソース確保
	Reset(m_insModel->GetInstanceMax());

	//シェーダをロード
	D3D_SHADER_MACRO macros[] = { "INSTANCING", "1", "MOTIONBLUR", "1", "ALBEDO_MAP", "1", NULL, NULL };
	m_psShader.Load("Preset/shader/citizen.fx", "PSMain_Citizen", Shader::EnType::PS, "PSMain_Citizen", macros);
	//シェーダをセット
	m_insModel->GetModelRender().GetSkinModel().FindMaterialSetting(
		[&](MaterialSetting* mat) {
			mat->SetPS(&m_psShader);
		}
	);
}

InstancingCitizenColorManager::~InstancingCitizenColorManager() {
	//シェーダリセット
	m_insModel->GetModelRender().GetSkinModel().FindMaterialSetting(
		[](MaterialSetting* mat) {
			mat->SetDefaultPS();
		}
	);
}

void InstancingCitizenColorManager::PreDraw(int instanceNum, int drawInstanceNum, const std::unique_ptr<bool[]>& drawInstanceMask) {
	//カリングされてないもののみコピー
	int drawNum = 0;
	for (int i = 0; i < instanceNum; i++) {
		if (drawInstanceMask[i]) {
			m_color[drawNum] = m_colorCache[i];
			drawNum++;
		}
	}
	//StructuredBufferを更新。
	GetGraphicsEngine().GetD3DDeviceContext()->UpdateSubresource(
		m_colorSB.Get(), 0, NULL, m_color.get(), 0, 0
	);
	//シェーダーリソースに色配列をセット
	GetGraphicsEngine().GetD3DDeviceContext()->PSSetShaderResources(enSkinModelSRVReg_Free, 1, m_colorSRV.GetAddressOf());
}
void InstancingCitizenColorManager::AddDrawInstance(int instanceIndex, const CMatrix& SRTMatrix, const CVector3& scale, void *param) {
	//カラーを設定
	m_colorCache[instanceIndex] = *(CVector4*)param;
}
void InstancingCitizenColorManager::SetInstanceMax(int instanceMax) {
	if (instanceMax > m_instanceMax) {
		Reset(instanceMax);
	}
}
