#include"DissolveSprite.hlsli"
Texture2D<float4> tex:register(t0);//0番スロットに設定されたテクスチャ
Texture2D<float4> tex2:register(t1);//0番スロットに設定されたテクスチャ
SamplerState smp:register(s0); //0番スロットに設定されたサンプラー

float4 main(VSOutput input) :SV_TARGET
{
	float4 texColor=tex.Sample(smp,input.uv);

	float4 maskColor=tex2.Sample(smp,input.uv);

	clip(maskColor.r-Dissolve);

	return texColor*color;
}