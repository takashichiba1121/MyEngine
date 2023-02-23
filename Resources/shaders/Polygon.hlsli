cbuffer cbuff0 : register(b0)
{
	//float4 color; // 色(RGBA)
	matrix mat; // ３Ｄ変換行列
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float3 pos : POSITION; // システム用頂点座標
	//float3 normal :NORMAL; // 法線ベクトル
	//float2 uv  :TEXCOORD; // uv値
	float4 color:COLOR;
};