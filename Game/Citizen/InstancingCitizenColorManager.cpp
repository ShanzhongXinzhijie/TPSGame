#include "stdafx.h"
#include "InstancingCitizenColorManager.h"
#include "DemolisherWeapon\Graphic\Model\SkinModelShaderConst.h"

void InstancingCitizenColorManager::Init(GameObj::InstancingModel* insModel) {
	m_insModel = insModel;
	
	//リソース確保
	m_instanceMax = m_insModel->GetInstanceMax();

	//色配列の確保
	m_color.reset(new CVector4[m_instanceMax]);

	//StructuredBufferの確保
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	int stride = sizeof(CVector4);
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = static_cast<UINT>(stride * m_instanceMax);
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = stride;
	GetEngine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&desc, NULL, &m_colorSB);
	
	//ShaderResourceViewの確保
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	descSRV.BufferEx.FirstElement = 0;
	descSRV.Format = DXGI_FORMAT_UNKNOWN;
	descSRV.BufferEx.NumElements = desc.ByteWidth / desc.StructureByteStride;
	GetEngine().GetGraphicsEngine().GetD3DDevice()->CreateShaderResourceView(m_colorSB, &descSRV, &m_colorSRV);

	//シェーダをロード
	D3D_SHADER_MACRO macros[] = { "INSTANCING", "1", "MOTIONBLUR", "1", "ALBEDO_MAP", "1", NULL, NULL };
	m_psShader.Load("Preset/shader/citizen.fx", "PSMain_Citizen", Shader::EnType::PS, "PSMain_Citizen", macros);
	//シェーダをセット
	m_insModel->GetModelRender().GetSkinModel().FindMaterialSetting(
		[&](MaterialSetting* mat) {
			mat->SetPS(&m_psShader);
		}
	);
	
	//インスタンシングモデルに関数セット
	m_insModel->SetPreDrawFunction(
		[&]() {
			//シェーダーリソースに色配列をセット
			GetEngine().GetGraphicsEngine().GetD3DDeviceContext()->PSSetShaderResources(enSkinModelSRVReg_Free, 1, &m_colorSRV);
		}
	);
}

void InstancingCitizenColorManager::Release() {
	//リソース解放
	m_color.reset();
	if (m_colorSB) { m_colorSB->Release(); m_colorSB = nullptr; }
	if (m_colorSRV) { m_colorSRV->Release(); m_colorSRV = nullptr; }

	//シェーダリセット
	m_insModel->GetModelRender().GetSkinModel().FindMaterialSetting(
		[](MaterialSetting* mat) {
			mat->SetDefaultPS();
		}
	);

	//関数リセット
	m_insModel->SetPreDrawFunction(nullptr);
	m_insModel = nullptr;

	m_instanceNum = 0; m_instanceMax = 0;
}

void InstancingCitizenColorManager::AddColor(const CVector4& color) {
	if (m_instanceMax <= m_instanceNum) { return; }
	m_color[m_instanceNum] = color;
	m_instanceNum++;
}
void InstancingCitizenColorManager::PostLoopPostUpdate() {
	if (m_instanceNum <= 0) { return; }

	//StructuredBufferを更新。
	GetEngine().GetGraphicsEngine().GetD3DDeviceContext()->UpdateSubresource(
		m_colorSB, 0, NULL, m_color.get(), 0, 0
	);

	m_instanceNum = 0;
}
