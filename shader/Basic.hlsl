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
   	float3 LightPosition1;
   	float3 LightPosition2;
   	float2 Padding;
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

	float3 LIGHTPPS = float3( 0.0f, 0.0f, 0.0f );
	Result.LightPos2 = LIGHTPPS.xyz - WorldPosition.xyz;
	Result.LightPos2 = normalize( Result.LightPos2 );

    return Result;
}

float4 PS(PS_INPUT Input) : SV_TARGET
{
	float4 TextureColor0 =  ObjTexture[0].Sample( ObjSamplerState, Input.TEXCOORD);
	float4 TextureColor1 =  ObjTexture[1].Sample( ObjSamplerState, Input.TEXCOORD);
	float4 TextureColor = TextureColor0 * TextureColor1 * 2;
	TextureColor = saturate( TextureColor );
	float3 LightDir = - LightDirection;
	float LightIntensity = saturate( dot( Input.NORMAL, LightDir ) );
	float4 Color = AmbientColor;
	float4 Specular = float4( 0.0f, 0.0f, 0.0f, 0.0f );

	if ( LightIntensity > 0.0f )
	{
		Color += ( DiffuseColor * LightIntensity );
		Color = saturate( Color );
		float3 Reflection = normalize( 2 * LightIntensity * Input.NORMAL - LightDir );

		Specular = pow( saturate( dot( Reflection, Input.ViewDirection ) ), SpecularPower );
	}

	Color = Color * TextureColor;

	float4 DiffuseColor1 = float4( 1.0f, 1.0f, 1.0f, 1.0f );
	float4 Color1 = DiffuseColor1 * saturate( dot( Input.NORMAL, Input.LightPos1 ) );

	float4 DiffuseColor2 = float4( 1.0f, 1.0f, 1.0f, 1.0f );
	float4 Color2 = DiffuseColor2 * saturate( dot( Input.NORMAL, Input.LightPos2 ) );

	Color = saturate( Color1 + Color2 );

	return Color;
}