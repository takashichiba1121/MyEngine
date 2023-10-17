cbuffer cbuff0 : register(b0) {
    //matrix mat; //3D変換行列
    matrix viewproj;
    matrix world;
    float3 cameraPos;
    float shininess;//光沢度
    float alpha;
    float3 color;
};


cbuffer cbuff1 : register(b1) {
    float3 m_ambient : packoffset(c0);  //アンビエント
    float3 m_diffuse : packoffset(c1);  //ディフューズ
    float3 m_specular : packoffset(c2); //スペキュラー
    float m_alpha : packoffset(c2.w); //アルファ
};

static const uint DIR_LIGHT_NUM = 3;

struct DirLight
{
    float3 lightv;//ライトへの方向の単位ベクトル
    float3 lightcolor;//ライトの色(RGB)
    uint active;
};

struct PointLight
{
    float3 lightpos;//ライトへの方向の単位ベクトル
    float3 lightcolor;//ライトの色(RGB)
    float3 lightAtten;//ライトへの方向の単位ベクトル
    uint active;
};

cbuffer cbuff2 : register(b2) {
    float3 ambientColor;
    DirLight dirLights[DIR_LIGHT_NUM];
    PointLight PointLights[DIR_LIGHT_NUM];
};

cbuffer cbuff3 : register(b3) {
    float _Destruction : packoffset(c0);
    float _ScaleFactor : packoffset(c0.y);
    float _RotationFactor : packoffset(c0.z);
    float _PositionFactor : packoffset(c0.w);
};

//頂点シェーダの出力構造体
//頂点シェーダからピクセルシェーダへのやり取りに使用する
struct VSOutput
{
    //システム用頂点座標
    float4 svpos : SV_POSITION;
    float3 normal:NORMAL;//法線ベクトル
    float4 worldpos:POSITION;
    //uv値
    float2 uv  :TEXCOORD;
};

struct GSOutput
{
    //システム用頂点座標
    float4 svpos : SV_POSITION;
    float3 normal:NORMAL;//法線ベクトル
    float4 worldpos:POSITION;
    //uv値
    float2 uv  :TEXCOORD;
};

