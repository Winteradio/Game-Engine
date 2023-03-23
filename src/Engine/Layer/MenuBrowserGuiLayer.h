#ifndef __MENUBROWSERGUILAYER_H__
#define __MENUBROWSERGUILAYER_H__

#include "IBrowserGuiLayer.h"

#include "ProjectManager.h"
#include "SettingBrowserGuiLayer.h"
#include "FileBrowserGuiLayer.h"

class MenuBrowserGuiLayer : public IBrowserGuiLayer
{
	public :
		MenuBrowserGuiLayer();
		MenuBrowserGuiLayer( const MenuBrowserGuiLayer& Other );
		virtual ~MenuBrowserGuiLayer();

	public :
		virtual void Init( int PosX, int PosY, int Width, int Height );
		virtual void Destroy();
		virtual void Update();
		virtual void Render();
		virtual void SetPath( FS::path Path );

	private :
		bool m_Mode;
		FS::path m_FilePath;
};

#endif // __MENUBROWSERGUILAYER_H__