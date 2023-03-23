#ifndef __IBROWSERGUILAYER_H__
#define __IBROWSERGUILAYER_H__

#include "IGuiLayer.h"
#include "FileDialog.h"

class IBrowserGuiLayer : public IGuiLayer
{
	public :
		IBrowserGuiLayer() {}
		IBrowserGuiLayer( const IBrowserGuiLayer& Other )
		{
			m_PosX = Other.m_PosX;
			m_PosY = Other.m_PosY;
			m_Width = Other.m_Width;
			m_Height = Other.m_Height;
			m_Name = Other.m_Name;
			m_Rendering = Other.m_Rendering;
		}
		virtual ~IBrowserGuiLayer() {}

	public :
		virtual void Init( int PosX, int PosY, int Width, int Height ) = 0;
		virtual void Destroy() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void SetPath( FS::path Path ) = 0;

	private :
};

#endif // __IBROWSERLAYER_H__