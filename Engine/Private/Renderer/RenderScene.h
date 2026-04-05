#ifndef __WTR_RENDERSCENE_H__
#define __WTR_RENDERSCENE_H__

#include <ECS/include/UUID/UUID.h>
#include <Container/include/HashMap.h>
#include <Container/include/DynamicArray.h>
#include <Memory/include/Pointer/RefPtr.h>
#include <Framework/Math/MathTypes.h>

namespace wtr
{
	class SceneProxy;
	class PrimitiveProxy;
	class LightProxy;
};

namespace wtr
{
	class RenderScene
	{
		public :
			template<typename T>
			using ProxyContainer = wtr::HashMap<ECS::UUID, Memory::RefPtr<T>>;
			
			using PendingProxy = wtr::DynamicArray<Memory::RefPtr<SceneProxy>>;
			
		public:
			RenderScene();
			~RenderScene();

		public :
			void Clear();
			void FlushPending();

			void UpdateProxy(const ECS::UUID& id, const fvec3 position, const fvec3 rotation, const fvec3 scale);

			void AddPrimitive(Memory::RefPtr<PrimitiveProxy> primitive);
			void RemovePrimitive(const ECS::UUID& id);
			Memory::RefPtr<PrimitiveProxy> GetPrimitive(const ECS::UUID& id);

			void AddLight(Memory::RefPtr<LightProxy> light);
			void RemoveLight(const ECS::UUID& id);
			Memory::RefPtr<LightProxy> GetLight(const ECS::UUID& id);

			PendingProxy& GetAddable();
			PendingProxy& GetRemovable();
			PendingProxy& GetUpdatable();

		private :
			void FlushAddable();
			void FlushRemovable();

		private :
			ProxyContainer<PrimitiveProxy> m_primitives;
			ProxyContainer<LightProxy> m_lights;

			PendingProxy m_addable;
			PendingProxy m_removable;
			PendingProxy m_updatable;
	};
};

#endif // __WTR_RENDERSCENE_H__