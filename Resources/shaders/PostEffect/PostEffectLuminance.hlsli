//���_�V�F�[�_�[�̏o�͍\����
//�i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����j
struct VSOutput
{
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	// uv�l
	float2 uv :TEXCOORD;
};
cbuffer cbuff0 : register(b0) {
	float3 m_color : packoffset(c0);
	float smoothstepMax : packoffset(c1.x);
	float smoothstepMin : packoffset(c1.y);
};