#include "RenderingEngine.h"

#include "CoreEngine.h"
#include "PhysicsEngine.h"

#include "LayerManager.h"
#include "ResourceManager.h"

RenderingEngine::RenderingEngine()
{

}

RenderingEngine::~RenderingEngine()
{
	Destroy();
}

RenderingEngine& RenderingEngine::Get()
{
	return m_RenderingEngine;
}

void RenderingEngine::Init()
{
	RenderingStatus::Get().Init();
}

void RenderingEngine::Begin()
{
	ImGuiAPI::Get().Begin();

	float Color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	DirectXAPI::Get().GetDeviceContext()->OMSetRenderTargets( 1, &RenderingStatus::Get().GetRenderTargetView(), RenderingStatus::Get().GetDepthStencilView() );
	DirectXAPI::Get().GetDeviceContext()->ClearRenderTargetView( RenderingStatus::Get().GetRenderTargetView(), Color );
	DirectXAPI::Get().GetDeviceContext()->ClearDepthStencilView( RenderingStatus::Get().GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
}

void RenderingEngine::Render()
{
	for ( IGuiLayer* Layer : LayerManager::Get().GetGuiLayer() )
	{
		if( Layer->GetRendering() ) Layer->Render();
	}

	// ImGui Demo Program
	ImGui::ShowDemoWindow();

	if( FileBrowserGuiLayer::Get().GetRendering() ) FileBrowserGuiLayer::Get().Render();
	if( SettingBrowserGuiLayer::Get().GetRendering() ) SettingBrowserGuiLayer::Get().Render();
}

void RenderingEngine::End()
{
	ImGuiAPI::Get().End();

	DirectXAPI::Get().GetSwapChain()->Present( 0, 0 );
}

void RenderingEngine::Destroy()
{

}

RenderingEngine RenderingEngine::m_RenderingEngine;