
//���_�V�F�[�_�[�̏o�͍\����
//�i���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����j
struct VSOutput
{
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	// uv�l
	float2 uv :TEXCOORD;
};
//3D�ϊ��s��
cbuffer ConstBufferDataTransform:register(b0)
{
	//matrix mat;//3D�ϊ��s��
}