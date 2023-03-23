#include "LayerManager.h"

LayerManager::LayerManager() {}

LayerManager::~LayerManager()
{
	Destroy();
}

LayerManager& LayerManager::Get() { return m_LayerManager; }

void LayerManager::Init( int Width, int Height )
{
	m_GuiLayerData.clear();
	m_GameLayerData.clear();


	MenuBrowserGuiLayer* MenuBrowserGui = new MenuBrowserGuiLayer();
	MenuBrowserGui->Init( 0, 0, Width, 0 );

	LogGuiLayer* LogGui = new LogGuiLayer();
	LogGui->Init( 0, 600, Width - 300, Height - 600 );

	PropertyBrowserGuiLayer* PropertyBrowserGui = new PropertyBrowserGuiLayer();
	PropertyBrowserGui->Init( Width - 300, 20, 300, Height - 20);

	ResourceGuiLayer* ResourceGui = new ResourceGuiLayer();
	ResourceGui->Init( 0, 20, 300, 600 - 20 );
	ResourceGui->SetPropertyBrowserGuiLayer( PropertyBrowserGui );

	PushGuiLayer( (IGuiLayer*)( MenuBrowserGui ) );
	PushGuiLayer( (IGuiLayer*)( LogGui ) );
	PushGuiLayer( (IGuiLayer*)( ResourceGui ) );
	PushGuiLayer( (IGuiLayer*)( PropertyBrowserGui ) );

	FileBrowserGuiLayer::Get().Init( 200, 200, 800, 400 );
	SettingBrowserGuiLayer::Get().Init( 200, 200, 600, 400 );
}

void LayerManager::Destroy()
{
	for ( auto Layer : m_GuiLayerData )
	{
		Layer->Destroy();
		delete Layer;
	}

	for ( auto Layer : m_GameLayerData )
	{
		Layer-> Destroy();
		delete Layer;
	}
}

void LayerManager::PushGameLayer( GameLayer* Layer )
{
	m_GameLayerData.push_back( Layer );
}

void LayerManager::PushGuiLayer( IGuiLayer* Layer )
{
	m_GuiLayerData.push_back( Layer );
}

GameLayer*& LayerManager::PopGameLayer() { return m_GameLayerData.back(); }
IGuiLayer*& LayerManager::PopGuiLayer() { return m_GuiLayerData.back();}

std::vector< IGuiLayer* >& LayerManager::GetGuiLayer() { return m_GuiLayerData; }
std::vector< GameLayer* >& LayerManager::GetGameLayer() { return m_GameLayerData; }

LayerManager LayerManager::m_LayerManager;