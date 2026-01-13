#ifndef __WTR_WGLCONTEXT_H__
#define __WTR_WGLCONTEXT_H__

#include <Windows.h>

namespace wtr
{
	class WGLContext
	{
	public :
		WGLContext();
		~WGLContext();

	public :
		bool Init(void* nativeHandle);
		bool MakeCurrent();
		void Present();

	private :
		bool InitPixel();
		bool InitContext();

	private :
		HDC m_deviceContext;
		HGLRC m_renderingContext;
	};
};

#endif // __WTR_WGLCONTEXT_H__