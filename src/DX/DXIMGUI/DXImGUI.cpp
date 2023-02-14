#include "DXImGUI.h"

DXIMGUI::DXIMGUI()
{
	InitPointer();
}

DXIMGUI::DXIMGUI( const DXIMGUI& Other )
{
	*this = Other;
}

DXIMGUI::~DXIMGUI(){}

bool DXIMGUI::Init( HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext )
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& IO = ImGui::GetIO();
	if ( !ImGui_ImplWin32_Init( hWnd ) )
	{
		LOG_ERROR(" Failed - Init Window Handle for IMGUI ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init Window Handle for IMGUI ");
	}

	if ( !ImGui_ImplDX11_Init( Device, DeviceContext ) )
	{
		LOG_ERROR(" Failed - Init DirectX11 Device and Device Context for IMGUI ");
		return false;
	}
	else
	{
		LOG_INFO(" Successed - Init DirectX11 Device and Device Context for IMGUI ");
	}
	ImGui::StyleColorsDark();
	return true;
}

bool DXIMGUI::Frame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	return true;
}

bool DXIMGUI::Render( RENDERSTATE* RenderState )
{
	static int Counter = 0;

	ImGui::SetNextWindowPos( ImVec2( 0, 0 ) );

	ImGui::Begin(" Rendering State ", NULL, ImGuiWindowFlags_AlwaysAutoResize );
	if ( ImGui::CollapsingHeader("Rasterizer State") )
	{
		ImGui::Spacing();
		if  ( ImGui::Button("No Cull") )
		{
			RenderState->RasterizerState = NOCUL;
		}
		ImGui::SameLine();
		if ( ImGui::Button("CW Cull") )
		{
			RenderState->RasterizerState = CWCUL;
		}
		ImGui::SameLine();
		if ( ImGui::Button("CCW Cull") )
		{
			RenderState->RasterizerState = CCWCUL;
		}
		ImGui::SameLine();
		if ( ImGui::Button("Wire Frame") )
		{
			RenderState->RasterizerState = WIREFRAME;
		}
	}

	if ( ImGui::CollapsingHeader("Blend State") )
	{
		ImGui::Spacing();
		if ( ImGui::Button("No Blend") )
		{
			RenderState->BlendState = NONE;
		}
		ImGui::SameLine();
		if ( ImGui::Button("Active Alpha") )
		{
			RenderState->BlendState = ACTIVEALPHA;
		}
		ImGui::SameLine();
		if ( ImGui::Button("Deactive Alpha") )
		{
			RenderState->BlendState = DEACTIVEALPHA;
		}
	}

	if ( ImGui::CollapsingHeader("Depth/Stencil State") )
	{
		ImGui::Spacing();
		if ( ImGui::Button("Active Depth") )
		{
			RenderState->DepthStencilState = DEPTHACTIVE;
		}
		ImGui::SameLine();
		if ( ImGui::Button("Deactive Depth") )
		{
			RenderState->DepthStencilState = DEPTHDEACTIVE;
		}
	}
	ImGui::End();
	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
	return true;
}

void DXIMGUI::Release()
{
	LOG_INFO(" Release - IMGUI ");
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DXIMGUI::InitPointer()
{

}