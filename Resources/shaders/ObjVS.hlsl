#include "Obj.hlsli"

VSOutput main(float4 pos : POSITION ,float3 normal : NORMAL ,float2 uv : TEXCOORD) {
	float4 wpos = mul(world, pos);

	VSOutput output;//�s�N�Z���V�F�[�_��n���l
	output.svpos = pos;
	output.normal = normal;
	output.worldpos = wpos;
	output.uv = uv+uvShift;
	return output;

}