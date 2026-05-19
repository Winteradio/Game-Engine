#ifndef __WTR_MOVESYSTEM_H__
#define __WTR_MOVESYSTEM_H__

#include <World/System/System.h>

namespace wtr
{
	class StaticMeshNode;
	class InstancedStaticMeshNode;
};

namespace wtr
{
	class MoveSystem : public TypedSystem<StaticMeshNode>
	{
	public :
		MoveSystem() = default;
		virtual ~MoveSystem() = default;

	public :
		void UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container) override;
	};

	class InstancedMoveSystem : public TypedSystem<InstancedStaticMeshNode>
	{
	public :
		InstancedMoveSystem() = default;
		virtual ~InstancedMoveSystem() = default;
		
	public :
		void UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container) override;
	};
};

#endif // __WTR_MOVESYSTEM_H__