#ifndef __NODE_H__
#define __NODE_H__

#include "Component.h"
#include "Entity.h"

struct INode
{
	public :
		INode() {}
		virtual ~INode() {}

	public :
		virtual void Init( Entity& Object ) = 0;
		std::string& GetID() { return m_ID.GetString(); }

	protected :
		MyUUID m_ID;
};

struct RenderableNode : public INode
{
	public :
		RenderableNode() : INode() {}
		RenderableNode( Entity& Object ) : INode()
		{
			Transform = Object.GetComponent<TransformComponent>();
			Mesh = Object.GetComponent<MeshComponent>();
			Material = Object.GetComponent<MaterialComponent>();

			m_ID = Object.GetID();
		}
		~RenderableNode() {}

	public :
		void Init( Entity& Object )
		{
			Transform = Object.GetComponent<TransformComponent>();
			Mesh = Object.GetComponent<MeshComponent>();
			Material = Object.GetComponent<MaterialComponent>();

			m_ID = Object.GetID();
		}

	public :
		int Transform;
		int Mesh;
		int Material;
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


	public :
		int Transform;
};

struct LightNode : public INode
{
	public :
		LightNode() : INode() {}
		LightNode( Entity& Object ) : INode()
		{
			Transform = Object.GetComponent<TransformComponent>();
			Color = Object.GetComponent<ColorComponent>();

			m_ID = Object.GetID();
		}
		~LightNode() {}

	public :
		void Init( Entity& Object )
		{
			Transform = Object.GetComponent<TransformComponent>();
			Color = Object.GetComponent<ColorComponent>();

			m_ID = Object.GetID();
		}


	public :
		int Transform;
		int Color;
};

#endif // __NODE_H__