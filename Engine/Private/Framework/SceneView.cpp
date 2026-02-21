#include <Framework/SceneView.h>

#include <Renderer/Proxy/MeshProxy.h>
#include <Renderer/Proxy/MaterialProxy.h>
#include <Renderer/Proxy/PrimitiveProxy.h>

namespace wtr
{
	void SceneView::Reset()
	{
		m_meshCommand.Reset();
		m_materialCommand.Reset();
		m_primitiveCommand.Reset();
	}
}