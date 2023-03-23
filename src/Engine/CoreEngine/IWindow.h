#ifndef __IWINDOW_H__
#define __IWINDOW_H__

#include "Log.h"

class IWindow
{
	public :
		IWindow() {};
		virtual ~IWindow() { m_TitleName.clear(); };

	public :
		int& GetPosX() { return m_PosX; }
		int& GetPosY() { return m_PosY; }
		int& GetWidth() { return m_Width; }
		int& GetHeight() { return m_Height;}
		std::string& GetName() { return m_TitleName; }

	protected :
		int m_PosX;
		int m_PosY;
		int m_Width;
		int m_Height;

		std::string m_TitleName;
};

#endif // __IWINDOW_H__