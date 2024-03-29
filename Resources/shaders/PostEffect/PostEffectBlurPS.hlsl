#include"PostEffectblur.hlsli"


Texture2D<float4> tex : register(t0);  	// 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      	// 0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV,float2 pickUV,float _Sigma)
{
	float d = distance(drawUV,pickUV);
	return exp(-(d * d) / (2 * _Sigma * _Sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
float totalWeight = 0;
float4 col = float4(0,0,0,0);

for (float py = -_Sigma; py <= _Sigma; py += _StepWidth)
{
	for (float px = -_Sigma; px <= _Sigma; px += _StepWidth)
	{
		float2 pickUV = input.uv + float2(px,py);
		float weight = Gaussian(input.uv,pickUV,_Sigma);
		col.rgb += tex.Sample(smp, pickUV).rgb * weight;
		totalWeight += weight;
	}
}
col.rgb = col.rgb / totalWeight;
col.a = 1;
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