//�C���X�^���V���O�s���p�s�N�Z���V�F�[�_

#include"model.fx"

StructuredBuffer<float4> Colors : register(t6);

PSOutput_RenderGBuffer PSMain_Citizen(PSInput In){//, uint instanceID : SV_InstanceID) {
	PSOutput_RenderGBuffer Out = PSMain_RenderGBuffer(In);
	
	Out.albedo *= Colors[In.instanceID];

	return Out;
}