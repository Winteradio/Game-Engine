#ifndef __WTR_SCENEVIEW_H__
#define __WTR_SCENEVIEW_H__

#include <Renderer/Proxy/ProxyList.h>
#include <Renderer/Proxy/MeshProxy.h>
#include <Renderer/Proxy/MaterialProxy.h>
#include <Renderer/Proxy/PrimitiveProxy.h>

namespace wtr
{
	class SceneView
	{
		public :
			SceneView() = default;
			~SceneView() = default;

		public :
			template<typename Proxy, typename... Args>
			Proxy* CreateMesh(Args&&... args)
			{
				return m_meshCommand.Create(std::forward<Args>(args)...);	
			}

			template<typename Proxy, typename... Args>
			Proxy* CreateMaterial(Args&&... args)
			{
				return m_materialCommand.Create(std::forward<Args>(args)...);
			}

			template<typename Proxy, typename... Args>
			Proxy* CreatePrimitive(Args&&... args)
			{
				return m_primitiveCommand.Create(std::forward<Args>(args)...);
			}

		public :
			void Reset();

		private :
			ProxyList<MeshProxy> m_meshCommand;
			ProxyList<MaterialProxy> m_materialCommand;
			ProxyList<PrimitiveProxy> m_primitiveCommand;
	};
};

#endif // __WTR_SCENEVIEW_H__