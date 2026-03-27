#ifndef __WTR_RENDERSCENE_H__
#define __WTR_RENDERSCENE_H__

#include <ECS/include/UUID/UUID.h>
#include <Container/include/HashMap.h>
#include <Container/include/DynamicArray.h>

namespace wtr
{
	class PrimitiveProxy;
	class LightProxy;
};

namespace wtr
{
	class RenderScene
	{
		public:
			RenderScene();
			~RenderScene();

		public :
			void UpdateProxy(const ECS::UUID& id);

			void AddPrimitive(Memory::RefPtr<PrimitiveProxy> primitive);
			void RemovePrimitive(const ECS::UUID& id);

			void AddLight(Memory::RefPtr<LightProxy> light);
			void RemoveLight(const ECS::UUID& id);

		private :
			wtr::HashMap<ECS::UUID, Memory::RefPtr<PrimitiveProxy>> m_primitives;
			wtr::HashMap<ECS::UUID, Memory::RefPtr<LightProxy>> m_lights;
	};
};

#endif // __WTR_RENDERSCENE_H__