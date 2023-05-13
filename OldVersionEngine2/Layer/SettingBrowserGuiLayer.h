#ifndef __SETTINGBROWSERGUILAYER_H__
#define __SETTINGBROWSERGUILAYER_H__

#include "IBrowserGuiLayer.h"

#include "RenderingStatus.h"

class SettingBrowserGuiLayer : public IBrowserGuiLayer
{
	public :
		SettingBrowserGuiLayer();
		SettingBrowserGuiLayer( const SettingBrowserGuiLayer& Other );
		virtual ~SettingBrowserGuiLayer();

	public :
		static SettingBrowserGuiLayer& Get();
		virtual void Init( int PosX, int PosY, int Width, int Height );
		virtual void Destroy();
		virtual void Update();
		virtual void Render();
		virtual void SetPath( FS::path Path );
		void Begin();
		void End();

	protected :
		static SettingBrowserGuiLayer m_SettingBrowserGuiLayer;
};

#endif // __SETTINGBROWSERGUILAYER_H__