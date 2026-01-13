#ifndef __WTR_NODE_H__
#define __WTR_NODE_H__

#include <ECS/include/Object/Data.h>
#include <Memory/include/Pointer/ObjectPtr.h>

#include <World/Component.h>
#include <Framework/Math/MathTypes.h>

namespace wtr
{
	namespace Node
	{
		struct Transform : ECS::Node::Base
		{
			Memory::ObjectPtr<Component::Transform> transform;
		};

		struct Camera : ECS::Node::Base
		{
			Memory::ObjectPtr<Component::Transform> transform;
			Memory::ObjectPtr<Component::Camera> camera;
		};

		struct Resource : ECS::Node::Base
		{
			Memory::ObjectPtr<Component::Resource> resource;
		};

		struct StaticMesh : Resource
		{
		};

		struct Material : Resource
		{

		};
	};
};

#endif // __WTR_NODE_H__