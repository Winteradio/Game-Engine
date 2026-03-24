#ifndef __WTR_RENDERSCENE_H__
#define __WTR_RENDERSCENE_H__

#include <ECS/include/UUID/UUID.h>
#include <Container/include/HashMap.h>

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
			void AddPrimitive(PrimitiveProxy* primitive);
			void RemovePrimitive(PrimitiveProxy* primitive);

			void AddLight(LightProxy* light);
			void RemoveLight(LightProxy* light);

		private :
			std::vector<PrimitiveProxy*> m_primitives;
			std::vector<LightProxy*> m_lights;
	};
};

#endif // __WTR_RENDERSCENE_H__