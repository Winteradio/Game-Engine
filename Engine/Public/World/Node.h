#ifndef __WTR_NODE_H__
#define __WTR_NODE_H__

#include <ECS/include/Object/Data.h>
#include <Memory/include/Pointer/ObjectPtr.h>
#include <Reflection/include/Type/TypeMacro.h>

#include <World/Component.h>
#include <Framework/Math/MathTypes.h>

namespace wtr
{
	class Scene;
};

namespace wtr
{
	class SceneNode : public ECS::Node
	{
		GENERATE(SceneNode);

	public :
		SceneNode() = default;
		virtual ~SceneNode() = default;

	public :
		void SetScene(Scene* owner);
		void Clear();

		void UpdatePosition(const fvec3& position);
		void UpdateRotation(const fvec3& rotation);
		void UpdateScale(const fvec3& scale);

		const fvec3 GetPosition() const;
		const fvec3 GetRotation() const;
		const fvec3 GetScale() const;

	protected :
		Memory::ObjectPtr<TransformComponent> transform;
		Scene* m_owner;
	};

	class LightNode : public SceneNode
	{
		GENERATE(LightNode);

	public :
		Memory::ObjectPtr<LightComponent> light;
	};

	class MeshNode : public SceneNode
	{
		GENERATE(MeshNode);

	public :
		Memory::ObjectPtr<MeshComponent> mesh;
	};

	class CameraNode : public ECS::Node
	{
		GENERATE(CameraNode);

	public :
		Memory::ObjectPtr<CameraComponent> camera;
	};
};

#endif // __WTR_NODE_H__