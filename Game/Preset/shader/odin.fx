#include"model.fx"

float4 PSMain(PSInput In) : SV_Target0
{
	return albedoScale;
}