#include "Obj.hlsli"

VSOutput main(float4 pos : POSITION ,float3 normal : NORMAL ,float2 uv : TEXCOORD) {
	//�E�A���A���̕����Ɍ��������C�g
	float3 lightdir = float3(1, -1, 1);
	lightdir = normalize(lightdir);
	//���C�g�̐F�i���j
	float3 lightColor = float3(1, 1, 1);

	float3 ambient = m_ambient;
	float3 diffuse = dot(-lightdir, normal) * m_diffuse;

	//���_���W
	const float3 eye = float3(0, 0, -20);
	//����x
	const float shininess = 4.0f;
	//���_���王�_�̃x�N�g��
	float3 eyedir = normalize(eye - pos.xyz);
	//���ˌ��x�N�g��
	float3 reflect = normalize(lightdir + 2 * dot(-lightdir, normal) * normal);
	//���ʔ��ˌ�
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	VSOutput output;//�s�N�Z���V�F�[�_��n���l
	output.svpos = mul(mat,pos);
	output.normal = normal;
	output.color.rgb = (ambient + diffuse + specular) * lightColor;
	output.color.a = m_alpha;
	output.uv = uv;
	return output;

}