#include"DissolveSprite.hlsli"
Texture2D<float4> tex:register(t0);//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex2:register(t1);//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp:register(s0); //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) :SV_TARGET
{
	float4 texColor=tex.Sample(smp,input.uv);

	float4 maskColor=tex2.Sample(smp,input.uv);

	float Emission = step(Dissolve-0.05,(maskColor.r+maskColor.g+maskColor.b)/3);

	clip(Emission-0.5);

	Emission = step(Dissolve, (maskColor.r + maskColor.g + maskColor.b) / 3);

	Emission =abs(Emission -1);

	return float4(texColor.rgb+Emission, texColor.a);
}