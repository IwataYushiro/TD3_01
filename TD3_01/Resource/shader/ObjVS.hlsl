#include "Obj.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD) {
	//法線にワールド行列によるスケーリング・回転を適用
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	//頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - wpos.xyz);

	//環境反射光
	float3 ambient = m_ambient;

	//拡散反射光
	float3 diffuse = dot(lightv, wnormal.xyz) * m_diffuse;

	//光沢度
	const float shininess = 4.0f;

	//反射光ベクトル
	float3 reflect = normalize(-lightv + 2 * dot(lightv, wnormal.xyz) * wnormal.xyz);

	//鏡面反射光
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	//ピクセルシェーダに渡す値
	VSOutput output;
	output.svpos = mul(mul(viewproj,world), pos);

	//Lambert
//	output.color.rgb = dot(-lightdir, normal) * m_diffuse * lightcolor;

	//Phong
	output.color.rgb = (ambient + diffuse + specular) * m_diffuse * lightcolor;


	output.color.a = m_alpha;
	output.uv = uv;

	return output;
}