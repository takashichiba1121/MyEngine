#include"PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0);  	// 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);      	// 0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
	float4 col1 = tex0.Sample(smp, input.uv);
	float4 col2 = tex1.Sample(smp, input.uv);

	float4 col = col1;
	if (fmod(input.uv.y, 0.1f) < 0.05f)
	{
		col = col2;
	}

	return float4(col.rgb, 1);
}