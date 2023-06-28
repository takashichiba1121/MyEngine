#include"PostEffectMultiRenderTarget.hlsli"

Texture2D<float4> tex : register(t0);  	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      	// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

PSOutput main(VSOutput input) : SV_TARGET
{
	PSOutput output;

float totalWeight = 0, _Sigma = 0.005, _StepWidth = 0.001;
float4 col = float4(0, 0, 0, 0);

for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
{
	for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
	{
		float2 pickUV = input.uv + float2(px, py);
		float weight = Gaussian(input.uv, pickUV, _Sigma);
		col.rgb += tex.Sample(smp, pickUV).rgb * weight;
		totalWeight += weight;
	}
}
col.rgb = col.rgb / totalWeight;
col.a = 1;

output.target0 = col;
output.target1 = float4(1-tex.Sample(smp, input.uv).rgb,1);


return output;
}
