#ifndef __NODES_H__
#define __NODES_H__

#include "Components.h"
#include "MyUUID.h"

struct INode
{
	public :
		INode() {}
		virtual ~INode() {}

	public :
		virtual void Init( Entity& Object ) = 0;

	private :
		MyUUID m_ID;
};

struct RenderableNode : public INode
{
	public :
		RenderableNode() : INode() {}
		RenderableNode( Entity& Object ) : INode()
		{
			Transform = Object.GetComponent<TransformComponent>();
			Mesh = Object.GetComponent<Mesh>();
			Material = Object.GetComponent<Material>();

			m_ID = Object.GetID();
		}
		~RenderableNode() {}

	public :
		void Init( Entity& Object )
		{
			Transform = Object.GetComponent<TransformComponent>();
			Mesh = Object.GetComponent<Mesh>();
			Material = Object.GetComponent<Material>();

			m_ID = Object.GetID();
		}

	private :
		TransformComponent* Transfom = nullptr;
		MeshComponent* Mesh = nullptr;
		MaterialComponent* Material = nullptr;
};

struct CameraNode : public INode
{
	public :
		CameraNode() : INode() {}
		CameraNode( Entity& Object ) : INode()
		{
			Transform = Object.GetComponent<TransformComponent>();

			m_ID = Object.GetID();
		}
		~CameraNode() {}

	public :
		void Init( Entity& Object )
		{
			Transform = Object.GetComponent<TransformComponent>();

			m_ID = Object.GetID();
		}


	private :
		TransformComponent* Transform = nullptr;
};

struct LightNode : public INode
{
	public :
		LightNode() : INode() {}
		LightNode( Entity& Object ) : INode()
		{
			Transform = Object.GetComponent<TransformComponent>();
			Color = Object.GetComponent<Color>();

			m_ID = Object.GetID();
		}
		~LightNode() {}

	public :
		void Init( Entity& Object )
		{
			Transform = Object.GetComponent<TransformComponent>();
			Color = Object.GetComponent<Color>();

			m_ID = Object.GetID();
		}


	private :
		TransformComponent* Transform = nullptr;
		ColorComponent* Color = nullptr;
};

#endif // __NODES_H__