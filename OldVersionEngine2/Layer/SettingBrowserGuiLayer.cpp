#include "SettingBrowserGuiLayer.h"

SettingBrowserGuiLayer::SettingBrowserGuiLayer()
{
	m_Name = "Setting";
	m_Rendering = false;
}

SettingBrowserGuiLayer::SettingBrowserGuiLayer( const SettingBrowserGuiLayer& Other )
{
	m_PosX = Other.m_PosX;
	m_PosY = Other.m_PosY;
	m_Width = Other.m_Width;
	m_Height = Other.m_Height;
	m_Rendering = Other.m_Rendering;
	m_Name = Other.m_Name;
}

SettingBrowserGuiLayer::~SettingBrowserGuiLayer()
{
	Destroy();
}

void SettingBrowserGuiLayer::Init( int PosX, int PosY, int Width, int Height )
{
	m_PosX = PosX;
	m_PosY = PosY;
	m_Width = Width;
	m_Height = Height;
}

void SettingBrowserGuiLayer::Destroy()
{
	
}

void SettingBrowserGuiLayer::Update()
{
	
}

void SettingBrowserGuiLayer::Render()
{
	ImGui::SetNextWindowPos( ImVec2( (float)m_PosX, (float)m_PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_Width, (float)m_Height ) );

	ImGui::Begin( m_Name.c_str(), &m_Rendering );
	if ( ImGui::CollapsingHeader("Rendering Status") )
	{
		if ( ImGui::TreeNode("Rasterizer State") )
		{
			const char* Items[] = {"Wire Frame", "Clock Wise Cull", "Counter Clock Wise Cull", "No Cull"};
			static int RasterIndex = 0;

			if ( ImGui::BeginCombo("State", Items[RasterIndex] ) )
			{
				if ( ImGui::Selectable("Wire Frame") )
				{
					RenderingStatus::Get().ChangeToRasterWireFrame();
					RasterIndex = 0;
				}
				if ( ImGui::Selectable("Clock Wise Cull") )
				{
					RenderingStatus::Get().ChangeToRasterCWCull();
					RasterIndex = 1;
				}
				if ( ImGui::Selectable("Counter Clock Wise Cull") )
				{
					RenderingStatus::Get().ChangeToRasterCCWCull();
					RasterIndex = 2;
				}
				if ( ImGui::Selectable("No Cull") )
				{
					RenderingStatus::Get().ChangeToRasterNoCull();
					RasterIndex = 3;
				}

				ImGui::EndCombo();
			}
			ImGui::TreePop();
		}

		if ( ImGui::TreeNode("Depth/Stencil State") )
		{
			const char* Items[] = {"Depth Enable", "Depth Disable"};
			static int DepthIndex = 0;

			if ( ImGui::BeginCombo("State", Items[DepthIndex] ) )
			{
				if ( ImGui::Selectable("Depth Enable") )
				{
					RenderingStatus::Get().ChangeToDepthStencilStateEnable();
					DepthIndex = 0;
				}
				if ( ImGui::Selectable("Depth Disable") )
				{
					RenderingStatus::Get().ChangeToDepthStencilStateDisable();
					DepthIndex = 1;
				}
				ImGui::EndCombo();
			}
			ImGui::TreePop();
		}

		if ( ImGui::TreeNode("Blend State") )
		{
			const char* Items[] = {"Blend Disable", "Alpha Blend Enable", "Alpha Blend Disable"};
			static int BlendIndex = 0;

			if ( ImGui::BeginCombo("State", Items[BlendIndex] ) )
			{
				if ( ImGui::Selectable("Blend Disable") )
				{
					RenderingStatus::Get().ChangeToBlendDisable();
					BlendIndex = 0;
				}
				if ( ImGui::Selectable("Alpha Blend Enable") )
				{
					RenderingStatus::Get().ChangeToBlendAlphaEnable();
					BlendIndex = 1;
				}
				if ( ImGui::Selectable("Alpha Blend Disable") )
				{
					RenderingStatus::Get().ChangeToBlendAlphaDisable();
					BlendIndex = 2;
				}

				ImGui::EndCombo();
			}
			ImGui::TreePop();
		}
	}
	if ( ImGui::CollapsingHeader("Font") )
	{

	}
	ImGui::End();
}

void SettingBrowserGuiLayer::SetPath( FS::path Path )
{

}

void SettingBrowserGuiLayer::Begin() { m_Rendering = true; }
void SettingBrowserGuiLayer::End() { m_Rendering = false; }

SettingBrowserGuiLayer& SettingBrowserGuiLayer::Get() { return m_SettingBrowserGuiLayer; }

SettingBrowserGuiLayer SettingBrowserGuiLayer::m_SettingBrowserGuiLayer;