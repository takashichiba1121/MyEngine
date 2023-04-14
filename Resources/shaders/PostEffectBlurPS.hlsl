#include"PostEffectblur.hlsli"


Texture2D<float4> tex : register(t0);  	// 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      	// 0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV,float2 pickUV,float sigma)
{
	float d = distance(drawUV,pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
float totalWeight = 0,_Sigma = 0.005,_StepWidth = 0.001;
float4 col = float4(0,0,0,0);

for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
{
	for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
	{
		float2 pickUV = input.uv + float2(px,py);
		float weight = Gaussian(input.uv,pickUV,_Sigma);
		col += tex.Sample(smp, pickUV) * weight;
		totalWeight += weight;
	}
}
col.rgb = col.rgb / totalWeight;
return col;
}

//Texture2D<float4> tex : register(t0);  	// 0番スロットに設定されたテクスチャ
//smpState smp : register(s0);      	// 0番スロットに設定されたサンプラー
//
//float3 sampleMainput(float2 uv) {
//    return tex.Sample(smp, uv).rgb;
//}
//
//float3 sampleBox(float2 uv, float delta) {
//    float4 _MainputTex_TexelSize = float4(0, 1, 0, 0);
//
//    float4 offset = _MainputTex_TexelSize.xyxy * float2(-delta, delta).xxyy;
//    float3 sum = sampleMainput(uv + offset.xy) + sampleMainput(uv + offset.zy) + sampleMainput(uv + offset.xw) + sampleMainput(uv + offset.zw);
//    return sum * 0.25;
//}
//
//float4 mainput(VSOutput inputput) : SV_TARGET
//{
//    float _inputtensity = 1;
//
//       float4 col = tex.Sample(smp, inputput.uv);
//                col.rgb += sampleBox(inputput.uv, 0.5) * _inputtensity;
//                return col;
//}