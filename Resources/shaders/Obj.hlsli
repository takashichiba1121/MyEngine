cbuffer cbuff0 : register(b0) {
    //matrix mat; //3D�ϊ��s��
    matrix viewproj;
    matrix world;
    float3 cameraPos;
    float shininess;//����x
    float alpha;
    float3 color;
};


cbuffer cbuff1 : register(b1) {
    float3 m_ambient : packoffset(c0);  //�A���r�G���g
    float3 m_diffuse : packoffset(c1);  //�f�B�t���[�Y
    float3 m_specular : packoffset(c2); //�X�y�L�����[
    float m_alpha : packoffset(c2.w); //�A���t�@
};

static const uint DIR_LIGHT_NUM = 3;

struct DirLight
{
    float3 lightv;//���C�g�ւ̕����̒P�ʃx�N�g��
    float3 lightcolor;//���C�g�̐F(RGB)
    uint active;
};

struct PointLight
{
    float3 lightpos;//���C�g�ւ̕����̒P�ʃx�N�g��
    float3 lightcolor;//���C�g�̐F(RGB)
    float3 lightAtten;//���C�g�ւ̕����̒P�ʃx�N�g��
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

//���_�V�F�[�_�̏o�͍\����
//���_�V�F�[�_����s�N�Z���V�F�[�_�ւ̂����Ɏg�p����
struct VSOutput
{
    //�V�X�e���p���_���W
    float4 svpos : SV_POSITION;
    float3 normal:NORMAL;//�@���x�N�g��
    float4 worldpos:POSITION;
    //uv�l
    float2 uv  :TEXCOORD;
};

struct GSOutput
{
    //�V�X�e���p���_���W
    float4 svpos : SV_POSITION;
    float3 normal:NORMAL;//�@���x�N�g��
    float4 worldpos:POSITION;
    //uv�l
    float2 uv  :TEXCOORD;
};

