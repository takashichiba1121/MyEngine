#include"PostEffectLuminance.hlsli"


Texture2D<float4> tex : register(t0);  	// 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      	// 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{

	//float offsetU = 2/ 1280.0f;
	//float offsetV = 2 / 720.0f;


	//float4 color = tex.Sample(smp, input.uv + float2(offsetU,0.0f));

	//color += tex.Sample(smp, input.uv + float2(-offsetU, 0.0f));

	//color += tex.Sample(smp, input.uv + float2(0.0f,offsetV));

	//color += tex.Sample(smp, input.uv + float2(0.0f, -offsetV));

	//color += tex.Sample(smp, input.uv + float2(offsetU, offsetV));
	//color += tex.Sample(smp, input.uv + float2(-offsetU, offsetV));
	//color += tex.Sample(smp, input.uv + float2(offsetU, -offsetV));
	//color += tex.Sample(smp, input.uv + float2(-offsetU, -offsetV));

	//color /= 9.0f;

	//color+= tex.Sample(smp, input.uv);

	//return float4(color.rgb, color.a);

	float4 col = tex.Sample(smp, input.uv);

	float grayScale = col.r *m_color.r + col.g * m_color.g + col.b * m_color.b;

	float extract = smoothstep(smoothstepMin, smoothstepMax, grayScale);
	col.a = extract;
	return col;
}

//Texture2D<float4> tex : register(t0);  	// 0番スロットに設定されたテクスチャ
//smpState smp : register(s0);      	// 0番スロットに設定されたサンプラー
//
//float3 sampleMain(float2 uv) {
//    return tex.Sample(smp, uv).rgb;
//}
//
//float3 sampleBox(float2 uv, float delta) {
//    float4 _MainTex_TexelSize = float4(0, 1, 0, 0);
//
//    float4 offset = _MainTex_TexelSize.xyxy * float2(-delta, delta).xxyy;
//    float3 sum = sampleMain(uv + offset.xy) + sampleMain(uv + offset.zy) + sampleMain(uv + offset.xw) + sampleMain(uv + offset.zw);
//    return sum * 0.25;
//}
//
//float4 main(VSOutput input) : SV_TARGET
//{
//    float _Intensity = 1;
//
//       float4 col = tex.Sample(smp, input.uv);
//                col.rgb += sampleBox(input.uv, 0.5) * _Intensity;
//                return col;
//}