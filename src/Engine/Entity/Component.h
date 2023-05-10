#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "DirectXAPI.h"
#include "MyUUID.h"

struct TagComponent
{
	std::string Tag;
};

struct IDComponent
{
	MyUUID ID;
};

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

struct ShaderComponent
{

};

struct RenderingStatusComponent
{
	ID3D11RenderTargetView* RenderTargetView = nullptr;
	ID3D11DepthStencilView* DepthStencilView = nullptr;
	ID3D11DepthStencilState* DepthStencilState = nullptr;
	ID3D11RasterizerState* RasterizerState = nullptr;
	ID3D11BlendState* BlendState = nullptr;
};

struct RenderableComponent
{
	bool Renderable = false;
};

#endif // __COMPONENT_H__