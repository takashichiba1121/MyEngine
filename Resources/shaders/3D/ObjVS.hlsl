#include "Obj.hlsli"

VSOutput main(float4 pos : POSITION ,float3 normal : NORMAL ,float2 uv : TEXCOORD) {
	float4 wpos = mul(world, pos);

	VSOutput output;//ピクセルシェーダを渡す値
	output.svpos = pos;
	output.normal = normal;
	output.worldpos = wpos;
	output.uv = uv+uvShift;
	return output;

}