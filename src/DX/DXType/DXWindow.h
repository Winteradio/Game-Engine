#ifndef __DXWINDOW_H__
#define __DXWINDOW_H__

struct DXRECTWINDOW
{
	const char* Name;
	int PosX;
	int PosY;
	int Width;
	int Height;
	bool Rendering;
};

#endif // __DXWINDOW_H__