#ifndef __ILAYER_H__
#define __ILAYER_H__

#include "Log.h"

class ILayer
{
	public :
		ILayer() {}
		ILayer( const ILayer& Other )
		{
			m_PosX = Other.m_PosX;
			m_PosY = Other.m_PosY;
			m_Width = Other.m_Width;
			m_Height = Other.m_Height;
			m_Name = Other.m_Name;
			m_Rendering = Other.m_Rendering;
		}
		virtual ~ILayer() {}

	public :
		virtual void Init( int PosX, int PosY, int Width, int Height ) = 0;
		virtual void Destroy() = 0;

		void SetRendering( bool Rendering ) { m_Rendering = Rendering; }

		int& GetPosX() { return m_PosX; }
		int& GetPosY() { return m_PosY; }
		int& GetWidth() { return m_Width; }
		int& GetHeight() { return m_Height;}
		bool& GetRendering() { return m_Rendering; }

	protected :
		int m_PosX;
		int m_PosY;
		int m_Width;
		int m_Height;
		std::string m_Name;
		bool m_Rendering;
};

#endif // __ILAYER_H__