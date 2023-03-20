#include "Particle.hlsli"

////�Z���^�[����̃I�t�Z�b�g
//static const float4 offset_array[8][3] =
//{
//	//��
//	{
//		float4(0,1,0,0),//����
//		float4(1,0,0,0),//����
//		float4(0,0,1,0)//�E��
//	},
//	{
//		float4(0,1,0,0),//����
//		float4(1,0,0,0),//����
//		float4(0,0,-1,0)//�E��
//	},
//	{
//		float4(0,1,0,0),//����
//		float4(-1,0,0,0),//����
//		float4(0,0,1,0)//�E��
//	},
//	{
//		float4(0,1,0,0),//����
//		float4(-1,0,0,0),//����
//		float4(0,0,-1,0)//�E��
//	},
//	//��
//	{
//		float4(0,-1,0,0),//����
//		float4(1,0,0,0),//����
//		float4(0,0,1,0)//�E��
//	},
//	{
//		float4(0,-1,0,0),//����
//		float4(1,0,0,0),//����
//		float4(0,0,-1,0)//�E��
//	},
//	{
//		float4(0,-1,0,0),//����
//		float4(-1,0,0,0),//����
//		float4(0,0,1,0)//�E��
//	},
//	{
//		float4(0,-1,0,0),//����
//		float4(-1,0,0,0),//����
//		float4(0,0,-1,0)//�E��
//	},
//};
//
//[maxvertexcount(8 * 3)]
//void main(
//	point VSOutput input[1] : SV_POSITION,
//	inout TriangleStream<GSOutput>output
//)
//{
//	float4 offset;
//	GSOutput element;
//	for (uint j = 0; j < 8; j++)
//	{
//		for (uint i = 0; i < 3; i++)
//		{
//			//offset = mul(matBillboard, offset_array[i]);
//			offset = offset_array[j][i] * input[0].scale;
//			//offset = mul(matBillboard, offset);
//			element.svpos = input[0].pos + offset;
//			//element.svpos = input[0].pos + offset_array[i];
//			element.svpos = mul(mat, element.svpos);
//			//element.uv = uv_array[i]; 
//			element.color = input[0].color;
//			output.Append(element);
//		}
//		output.RestartStrip();
//	}
//}

//�Z���^�[����̃I�t�Z�b�g
static const float4 offset_array[20][3] =
{
	{
		float4(1  ,0  ,0.5,0),//����
		float4(0.5,1  ,0  ,0),//����
		float4(0  ,0.5,1  ,0),//����
	},
	{
		float4(-1  ,0  ,0.5,0),//����
		float4(-0.5,1  ,0  ,0),//����
		float4(0  ,0.5  ,1,0)//�E��
	},
	{
		float4(1  , 0  ,0.5,0),//����
		float4(0.5,-1  ,0  ,0),//����
		float4(0  ,-0.5,1  ,0),//����
	},
	{
		float4(-1  , 0  ,0.5,0),//����
		float4(-0.5,-1  ,0  ,0),//����
		float4(0   ,-0.5,1  ,0),//����
	},

	{
		float4(0.5,1  ,0,0),//����
		float4(-0.5,1  ,0,0),//����
		float4(0  ,0.5,1,0),//����
	},
	{
		float4(0.5,1  , 0,0),//����
		float4(-0.5,1  , 0,0),//����
		float4(0   ,0.5,-1,0),//����
	},
	{
		float4(0.5,-1  ,0,0),//����
		float4(-0.5,-1  ,0,0),//����
		float4(0  ,-0.5,1,0),//����
	},
	{
		float4(0.5,-1  , 0,0),//����
		float4(-0.5,-1  , 0,0),//����
		float4(0  ,-0.5,-1,0)//�E��
	},

	{
		float4(1  ,0, 0.5,0),//����
		float4(1  ,0,-0.5,0),//����
		float4(0.5,1, 0  ,0)//�E��
	},
	{
		float4(1  , 0, 0.5,0),//����
		float4(1  , 0,-0.5,0),//����
		float4(0.5,-1, 0  ,0)//�E��
	},
	{
		float4(-1  ,0, 0.5,0),//����
		float4(-1  ,0,-0.5,0),//����
		float4(-0.5,1, 0  ,0)//�E��
	},
	{
		float4(-1  , 0, 0.5,0),//����
		float4(-1  , 0,-0.5,0),//����
		float4(-0.5,-1, 0  ,0)//�E��
	},

	{
		float4(0, 0.5,1  ,0),//����
		float4(0,-0.5,1  ,0),//����
		float4(1, 0  ,0.5,0),//����
	},
	{
		float4( 0, 0.5,1  ,0),//����
		float4( 0,-0.5,1  ,0),//����
		float4(-1, 0  ,0.5,0),//����
	},
	{
		float4(0, 0.5,-1  ,0),//����
		float4(0,-0.5,-1  ,0),//����
		float4(1, 0  ,-0.5,0)//�E��
	},
	{
		float4( 0, 0.5,-1  ,0),//����
		float4( 0,-0.5,-1  ,0),//����
		float4(-1, 0  ,-0.5,0)//�E��
	},

	{
		float4(1  , 0  ,-0.5,0),//����
		float4(0.5, 1  , 0  ,0),//����
		float4(0  ,0.5,-1  ,0),//����
	},
	{
		float4(-1  ,0  ,-0.5,0),//����
		float4(-0.5,1  , 0  ,0),//����
		float4( 0  ,0.5,-1  ,0),//����
	},
	{
		float4(1  , 0  ,-0.5,0),//����
		float4(0.5,-1  , 0  ,0),//����
		float4(0  ,-0.5,-1  ,0),//����
	},
	{
		float4(-1  , 0  ,-0.5,0),//����
		float4(-0.5,-1  , 0  ,0),//����
		float4( 0  ,-0.5,-1  ,0),//����
	},
};

static const float2 uv_array[3] =
{
	float2(0.0,1.0),
	float2(0.5,0.0),
	float2(1.0,1.0),
};

[maxvertexcount(20 * 3)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream<GSOutput>output
)
{
	float4 offset;
	GSOutput element;
	for (uint j = 0; j < 20; j++)
	{
		for (uint i = 0; i < 3; i++)
		{
			//offset = mul(matBillboard, offset_array[i]);
			offset = offset_array[j][i] * input[0].scale;
			//offset = mul(matBillboard, offset);
			element.svpos = input[0].pos + offset;
			//element.svpos = input[0].pos + offset_array[i];
			element.svpos =  element.svpos;
			element.uv = uv_array[i]; 
			element.color = input[0].color;
			output.Append(element);
		}
		output.RestartStrip();
	}
}

