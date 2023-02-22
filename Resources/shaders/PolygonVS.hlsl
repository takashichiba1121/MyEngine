#include "Polygon.hlsli"

VSOutput main(float3 pos : POSITION,float4 color:COLOR)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.pos = mul(mat, pos);
	//output.scale = scale;
	output.color = color;
	/*output.normal = normal;
	output.uv = uv;*/
	return output;
}