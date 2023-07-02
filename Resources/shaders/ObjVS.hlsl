#include "Obj.hlsli"

VSOutput main(float4 pos : POSITION ,float3 normal : NORMAL ,float2 uv : TEXCOORD) {
	////�E�A���A���̕����Ɍ��������C�g
	//float3 lightdir = float3(1, -1, 1);
	//lightdir = normalize(lightdir);
	////���C�g�̐F�i���j
	//float3 lightColor = float3(1, 1, 1);
	float4 wpos = mul(world, pos);

	VSOutput output;//�s�N�Z���V�F�[�_��n���l
	output.svpos = pos;
	output.normal = normal;
	output.worldpos = wpos;
	output.uv = uv;
	return output;

}