#include "Obj.hlsli"

VSOutput main(float4 pos : POSITION ,float3 normal : NORMAL ,float2 uv : TEXCOORD) {
	////右、下、奥の方向に向いたライト
	//float3 lightdir = float3(1, -1, 1);
	//lightdir = normalize(lightdir);
	////ライトの色（白）
	//float3 lightColor = float3(1, 1, 1);
	float4 wpos = mul(world, pos);

	VSOutput output;//ピクセルシェーダを渡す値
	output.svpos = pos;
	output.normal = normal;
	output.worldpos = wpos;
	output.uv = uv;
	return output;

}