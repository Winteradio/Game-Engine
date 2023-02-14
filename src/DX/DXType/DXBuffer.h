#ifndef __DXBUFFERTYPE_H__
#define __DXBUFFERTYPE_H__

#include "DXCommon.h"

typedef struct MATRIXBUFFER
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;
	XMFLOAT4 LightPosition1;
	XMFLOAT4 LightPosition2;
}MATRIXBUFFER;

typedef struct CAMERABUFFER
{
	XMFLOAT3 Position;
	XMFLOAT3 Target;
	XMFLOAT3 Up;
	XMFLOAT3 Padding;
}CAMERA;

typedef struct LIGHTBUFFER
{
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
	XMFLOAT3 LightDirection;
	float SpecularPower;
}LIGHTBUFFER;

typedef struct VERTEXBUFFER
{
	VERTEXBUFFER(){}
	VERTEXBUFFER( float X, float Y, float Z,
		float U, float V,
		float NX, float NY, float NZ ) : POS( X, Y, Z ), TEXCOORD( U, V ), NORMAL( NX, NY, NZ ){}

	XMFLOAT3 POS;
	XMFLOAT2 TEXCOORD;
	XMFLOAT3 NORMAL;
}VERTEX;

#endif // __DXBUFFERTYPE_H__