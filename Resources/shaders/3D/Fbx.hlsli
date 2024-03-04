cbuffer cbuff0 : register(b0) {
    matrix mat; //3D変換行列
};


cbuffer cbuff1 : register(b1) {
    float3 m_ambient : packoffset(c0);  //アンビエント
    float3 m_diffuse : packoffset(c1);  //ディフューズ
    float3 m_specular : packoffset(c2); //スペキュラー
    float m_alpha : packoffset(c2.w); //アルファ
};

cbuffer cbuff2 : register(b2) {
    float _Destruction : packoffset(c0);
    float _ScaleFactor : packoffset(c0.y);
    float _RotationFactor : packoffset(c0.z);
    float _PositionFactor : packoffset(c0.w);
    float _AlphaFactor : packoffset(c1);
};

//頂点シェーダの出力構造体
//頂点シェーダからピクセルシェーダへのやり取りに使用する
struct VSOutput
{
    //システム用頂点座標
    float4 svpos : SV_POSITION;
    float3 normal:NORMAL;//法線ベクトル
    float4 color:COLOR;
    //uv値
    float2 uv  :TEXCOORD;
};

struct GSOutput
{
    //システム用頂点座標
    float4 svpos : SV_POSITION;
    float3 normal:NORMAL;//法線ベクトル
    float4 color:COLOR;
    //uv値
    float2 uv  :TEXCOORD;
};


