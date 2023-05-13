#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "DirectXAPI.h"
#include "MyUUID.h"

struct IComponent
{
	virtual ~IComponent() = default;
};

struct TagComponent : public IComponent
{
	std::string Tag;

	TagComponent() = default;
	TagComponent( const TagComponent& Other ) = default;
	TagComponent( const std::string& Name ) : Tag( Name ) {};
	~TagComponent() = default;
};

struct TransformComponent : public IComponent
{
	XMFLOAT3 Translation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3 Rotation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3 Scale = XMFLOAT3( 0.0f, 0.0f, 0.0f );

	TransformComponent() = default;
	TransformComponent( const TransformComponent& Other ) = default;
	TransformComponent( const XMFLOAT3& Trans ) : Translation( Trans ) {};
	TransformComponent( const XMFLOAT3& Trans, const XMFLOAT3& Rot ) : Translation( Trans ), Rotation( Rot ) {};
	TransformComponent( const XMFLOAT3& Trans, const XMFLOAT3& Rot, const XMFLOAT3& Scale ) : Translation( Trans ), Rotation( Rot ), Scale( Scale ) {};
	~TransformComponent() = default;
};

struct ColorComponent : public IComponent
{
	XMFLOAT4 Color = XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );

	ColorComponent() = default;
	ColorComponent( const ColorComponent& Other ) = default;
	ColorComponent( const XMFLOAT4& Color ) : Color( Color ) {};
	~ColorComponent() = default;
};

struct MeshComponent : public IComponent
{

};

struct MaterialComponent : public IComponent
{

};

struct ShaderComponent : public IComponent
{

};

struct RenderableComponent : public IComponent
{
	bool Renderable = false;
};

#endif // __COMPONENT_H__