// Constant Buffer Size must be 16 *
cbuffer LIGHTBUFFER
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 LightDirection;
	float SpecularPower;
};

cbuffer MATRIXBUFFER
{
    matrix World;
    matrix View;
    matrix Projection;
   	float4 LightPosition1;
   	float4 LightPosition2;
};

cbuffer CAMERABUFFER
{
	float3 CameraPosition;
	float3 CameraTarget;
	float3 CameraUp;
	float3 CameraPadding;
};

Texture2D ObjTexture[2];
SamplerState ObjSamplerState;

struct PS_INPUT
{
	float4 POS : SV_POSITION;
	float2 TEXCOORD : TEXCOORD0;
	float3 NORMAL : NORMAL;
	float3 ViewDirection : TEXCOORD1;
	float3 LightPos1 : TEXCOORD2;
	float3 LightPos2 : TEXCOORD3;
};

PS_INPUT VS( float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 innormal : NORMAL )
{
	PS_INPUT Result;

	Result.POS.w = 1.0f;

	Result.POS = mul( inPos, World );
	Result.POS = mul( Result.POS, View );
	Result.POS = mul( Result.POS, Projection );

	Result.TEXCOORD = inTexCoord;

	Result.NORMAL = mul( innormal, (float3x3)World );
	Result.NORMAL = normalize( Result.NORMAL );

	float4 WorldPosition = mul( inPos, World );

	Result.ViewDirection = CameraPosition.xyz - WorldPosition.xyz;
	Result.ViewDirection = normalize( Result.ViewDirection );

	Result.LightPos1 = LightPosition1.xyz - WorldPosition.xyz;
	Result.LightPos1 = normalize( Result.LightPos1 );

	Result.LightPos2 = LightPosition2.xyz - WorldPosition.xyz;
	Result.LightPos2 = normalize( Result.LightPos2 );

    return Result;
}

float4 PS(PS_INPUT Input) : SV_TARGET
{
	float4 TextureColor0 =  ObjTexture[0].Sample( ObjSamplerState, Input.TEXCOORD);
	float4 TextureColor1 =  ObjTexture[1].Sample( ObjSamplerState, Input.TEXCOORD);

	return TextureColor0;
}