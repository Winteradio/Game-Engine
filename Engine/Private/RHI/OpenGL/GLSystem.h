#ifndef __WTR_GLSYSTEM_H__
#define __WTR_GLSYSTEM_H__

#include <RHI/RHISystem.h>
#include <RHI/OpenGL/Win32/WGLContext.h>

namespace wtr
{
	class GLSystem : public RHISystem
	{
	public :
		GLSystem();
		~GLSystem();

	public :
		bool Init(void* nativeHandle) override;

	private :
		WGLContext m_context;
	};
};

#endif // __WTR_GLSYSTEM_H__