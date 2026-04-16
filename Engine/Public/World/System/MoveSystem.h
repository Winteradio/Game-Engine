#ifndef __WTR_MOVESYSTEM_H__
#define __WTR_MOVESYSTEM_H__

#include <World/System/System.h>

namespace wtr
{
	class MoveSystem : public TypedSystem<MeshNode>
	{
	public :
		MoveSystem() = default;
		virtual ~MoveSystem() = default;

	public :
		void UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container) override;
	};
};

#endif // __WTR_MOVESYSTEM_H__