#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "DirectXAPI.h"

struct TransformComponent
{
	XMFLOAT3 Translation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3 Rotation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3 Scale = XMFLOAT3( 0.0f, 0.0f, 0.0f );
};

struct ColorComponent
{
	XMFLOAT4 Color = XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
};

struct MeshComponent
{

};

struct MaterialComponent
{

};


#endif // __COMPONENT_H__