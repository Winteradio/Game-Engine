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

bool DXIMGUI::Render()
{
	static int Counter = 0;

	ImGui::Begin(" Test ");
	ImGui::Text(" This is example text. ");

	if ( ImGui::Button("Click Me! ") )
	{
		Counter += 1;
	}
	std::string ClickCount = "ClickCount : " + std::to_string( Counter );
	ImGui::Text( ClickCount.c_str() );
	ImGui::End();
	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
	return true;
}

void DXIMGUI::Release()
{

}

void DXIMGUI::InitPointer()
{

}