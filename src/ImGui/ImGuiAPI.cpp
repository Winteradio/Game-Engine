#include "ImGuiAPI.h"

ImGuiAPI::ImGuiAPI()
{
	m_ImGuiIO = nullptr;
}

ImGuiAPI::~ImGuiAPI()
{
	Destroy();
}

ImGuiAPI& ImGuiAPI::Get() { return m_ImGuiAPI; }

void ImGuiAPI::Init( HWND hWnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext )
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	m_ImGuiIO = &ImGui::GetIO();

	if ( !ImGui_ImplWin32_Init( hWnd ) ) { return Log::Error(" Failed - Init ImGui using Window Handle "); }
	else { Log::Info(" Succseed - Init ImGui using Window Handle " );}

	if ( !ImGui_ImplDX11_Init( Device, DeviceContext ) ) { return Log::Error(" Failed - Init ImGui using DirectX Device and Context "); }
	else { Log::Info(" Successed - Init ImGui using DirectX Device and Context "); }
}

void ImGuiAPI::Destroy()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiAPI::Begin()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiAPI::End()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
}

void SetFont()
{

}
void SetStyle()
{

}

ImGuiIO*& ImGuiAPI::GetImGuiIO() { return m_ImGuiIO; }

ImGuiAPI ImGuiAPI::m_ImGuiAPI;