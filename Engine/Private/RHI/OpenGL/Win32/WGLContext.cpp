#include <RHI/OpenGL/Win32/WGLContext.h>

#include <Log/include/Log.h>
#include <glad/glad_wgl.h>
#include <wingdi.h>

namespace wtr
{
	WGLContext::WGLContext()
		: m_deviceContext()
		, m_renderingContext()
	{}

	WGLContext::~WGLContext()
	{}

	bool WGLContext::Init(void* nativeHandle)
	{
		if (nullptr == nativeHandle)
		{
			return false;
		}

		const HWND windowHandle = static_cast<const HWND>(nativeHandle);

		m_deviceContext = GetDC(windowHandle);

		if (!InitPixel())
		{
			LOGERROR() << "[WGL] Failed to initialize the pixel format";
			return false;
		}

		if (!InitContext())
		{
			LOGERROR() << "[WGL] Failed to initialize the rendering context";
			return false;
		}

		if (!MakeCurrent())
		{
			LOGERROR() << "[WGL] Failed to make current context";
			return false;
		}

		if (!gladLoadGL())
		{
			LOGERROR() << "[WGL] Failed to load the OpenGL function symbols";
			return false;
		}

		LOGINFO() << "[WGL] Succeed to initialize the OpenGL Context";

		return true;
	}

	bool WGLContext::MakeCurrent()
	{
		if (!wglMakeCurrent(m_deviceContext, m_renderingContext))
		{
			LOGERROR() << "[WGL] Unable to apply OpenGL context to window";
			return false;
		}

		return true;
	}

	void WGLContext::Present()
	{
		SwapBuffers(m_deviceContext);
	}

	bool WGLContext::InitPixel()
	{
		if (!m_deviceContext)
		{
			LOGERROR() << "[WGL] Failed to initialize the device pixel format, cause the device context is invalid";
			return false;
		}

		PIXELFORMATDESCRIPTOR pixelFormatDesc;
		ZeroMemory(&pixelFormatDesc, sizeof(pixelFormatDesc));

		pixelFormatDesc.nSize			= sizeof(pixelFormatDesc);
		pixelFormatDesc.nVersion		= 1;
		pixelFormatDesc.iPixelType		= PFD_TYPE_RGBA;
		pixelFormatDesc.dwFlags			= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pixelFormatDesc.cColorBits		= 32;
		pixelFormatDesc.cAlphaBits		= 8;
		pixelFormatDesc.iLayerType		= PFD_MAIN_PLANE;
		pixelFormatDesc.cDepthBits		= 24;
		pixelFormatDesc.cStencilBits	= 8;

		int pixelFormatID = ChoosePixelFormat(m_deviceContext, &pixelFormatDesc);
		if (!pixelFormatID)
		{
			LOGERROR() << "[WGL] Unable to find a suitable pixel format for the requested description";
			return false;
		}

		if (!SetPixelFormat(m_deviceContext, pixelFormatID, &pixelFormatDesc))
		{
			LOGERROR() << "[WGL] Failed to set the pixel format";
			return false;
		}

		LOGINFO() << "[WGL] Succeed to initialize the pixel format";
		return true;
	}

	bool WGLContext::InitContext()
	{
		if (!m_deviceContext)
		{
			LOGERROR() << "[WGL] Failed to initialize the rendering context, cause the device context is invalid";
			return false;
		}

		int attribList[] = 
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 5,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0,
		};

		HGLRC dummyContext = wglCreateContext(m_deviceContext);
		if (!dummyContext)
		{
			LOGERROR() << "[WGL] Failed to create the dummy rendering context";
			return false;
		}

		if (!wglMakeCurrent(m_deviceContext, dummyContext))
		{
			LOGERROR() << "[WGL] Failed to make current using dummy rendering context";
			return false;
		}

		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if (wglCreateContextAttribsARB)
		{
			m_renderingContext = wglCreateContextAttribsARB(m_deviceContext, nullptr, attribList);
		}

		wglDeleteContext(dummyContext);

		if (!m_renderingContext)
		{
			LOGERROR() << "[WGL] Failed to create the rendering context";
			return false;
		}

		LOGINFO() << "[WGL] Succeed to initialize the rendering context";
		return true;
	}
}