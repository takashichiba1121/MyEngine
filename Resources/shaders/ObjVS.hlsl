#include "Obj.hlsli"

VSOutput main(float4 pos : POSITION ,float3 normal : NORMAL ,float2 uv : TEXCOORD) {
	////右、下、奥の方向に向いたライト
	//float3 lightdir = float3(1, -1, 1);
	//lightdir = normalize(lightdir);
	////ライトの色（白）
	//float3 lightColor = float3(1, 1, 1);
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	float3 eyedir = normalize(cameraPos - wpos.xyz);

	float3 ambient = m_ambient;
	float3 diffuse = dot(m_lightv, wnormal.xyz) * m_diffuse;

	//視点座標
	const float3 eye = float3(0, 0, -20);
	//光沢度
	const float shininess = 4.0f;
	//頂点から視点のベクトル
	//反射光edirベクトル
	float3 reflect = normalize(-m_lightv + 2 * dot(-m_lightv, wnormal.xyz) * wnormal.xyz);
	//鏡面反射光
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	VSOutput output;//ピクセルシェーダを渡す値
	output.svpos = pos;
	output.normal = normal;
	output.color.rgb = (ambient + diffuse + specular) * float4(1,1,1,1);
	output.color.a = m_alpha;
	output.uv = uv;
	return output;

}