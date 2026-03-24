#ifndef __WTR_SYSTEM_H__
#define __WTR_SYSTEM_H__

#include <ECS/include/Object/System.h>
#include <World/Node.h>

namespace wtr
{
	class CameraSystem : public ECS::TypedSystem<CameraNode>
	{
	public :
		CameraSystem();
		virtual ~CameraSystem();

		void UpdateInternal(const ECS::TimeStep& timeStep, Memory::ObjectPtr<ContainerType> container) override;
	};
};

#endif // __WTR_SYSTEM_H__