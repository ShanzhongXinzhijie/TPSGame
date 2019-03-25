#include"model.fx"

float4 PSMain(PSInput In) : SV_Target0
{
	return float4(albedoScale.rgb, saturate((In.Viewpos.z-150.0f)/500.0f));
}