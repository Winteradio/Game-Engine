#ifndef __LAYERMANAGER_H__
#define __LAYERMANAGER_H__

// "ILayer.h"
#include "GameLayer.h"

// "IGuiLayer.h"
#include "LogGuiLayer.h"
#include "ResourceGuiLayer.h"

// "IBrowserGuiLayer.h"
#include "FileBrowserGuiLayer.h"
#include "MenuBrowserGuiLayer.h"
#include "PropertyBrowserGuiLayer.h"
#include "SettingBrowserGuiLayer.h"

class LayerManager
{
	private :
		LayerManager();
		~LayerManager();

	public :
		static LayerManager& Get();
		void Init( int Width, int Height );
		void Destroy();

		void PushGameLayer( GameLayer* Layer );
		void PushGuiLayer( IGuiLayer* Layer );

		GameLayer*& PopGameLayer();
		IGuiLayer*& PopGuiLayer();

		std::vector< IGuiLayer* >& GetGuiLayer();
		std::vector< GameLayer* >& GetGameLayer();

	private :
		static LayerManager m_LayerManager;

		std::vector< IGuiLayer* > m_GuiLayerData;
		std::vector< GameLayer* > m_GameLayerData;

};

#endif // _LAYERMANAGER_H__