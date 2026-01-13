#include <RHI/OpenGL/GLSystem.h>

#include <Log/include/Log.h>

namespace wtr
{
	GLSystem::GLSystem()
		: m_context()
	{}

	GLSystem::~GLSystem()
	{}

	bool GLSystem::Init(void* nativeHandle)
	{
		if (!m_context.Init(nativeHandle))
		{
			LOGERROR() << "[GL] Failed to initialize the OpenGL, cause failed to initialize the WGL";
			return false;
		}

		return true;
	}
}