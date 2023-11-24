//頂点シェーダーの出力構造体
//（頂点シェーダーからピクセルシェーダーへのやり取りに使用する）
struct VSOutput
{
	//システム用頂点座標
	float4 svpos : SV_POSITION;
	// uv値
	float2 uv :TEXCOORD;
};
cbuffer cbuff0 : register(b0) {
	float3 m_color : packoffset(c0);
	float smoothstepMax : packoffset(c1.x);
	float smoothstepMin : packoffset(c1.y);
};