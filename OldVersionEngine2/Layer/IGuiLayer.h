#ifndef __IGUILAYER_H__
#define __IGUILAYER_H__

#include "ILayer.h"
#include "ImGuiAPI.h"

class IGuiLayer : public ILayer
{
	public :
		IGuiLayer() {}
		IGuiLayer( const IGuiLayer& Other )
		{
			m_PosX = Other.m_PosX;
			m_PosY = Other.m_PosY;
			m_Width = Other.m_Width;
			m_Height = Other.m_Height;
			m_Name = Other.m_Name;
			m_Rendering = Other.m_Rendering;
		}
		virtual ~IGuiLayer() {}

	public :
		virtual void Init( int PosX, int PosY, int Width, int Height ) = 0;
		virtual void Destroy() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;

	private :
};

#endif // __IGUILAYER_H__
