#include "PropertyBrowserGuiLayer.h"

PropertyBrowserGuiLayer::PropertyBrowserGuiLayer()
{
	m_Name = "Property";
	m_Rendering = true;
	m_Resource = nullptr;
}

PropertyBrowserGuiLayer::PropertyBrowserGuiLayer( const PropertyBrowserGuiLayer& Other )
{
	m_PosX = Other.m_PosX;
	m_PosY = Other.m_PosY;
	m_Width = Other.m_Width;
	m_Height = Other.m_Height;
	m_Rendering = Other.m_Rendering;

	m_Name = Other.m_Name;
}

PropertyBrowserGuiLayer::~PropertyBrowserGuiLayer()
{
	Destroy();
}

void PropertyBrowserGuiLayer::Init( int PosX, int PosY, int Width, int Height )
{
	m_PosX = PosX;
	m_PosY = PosY;
	m_Width = Width;
	m_Height = Height;
}

void PropertyBrowserGuiLayer::Destroy()
{

}

void PropertyBrowserGuiLayer::Update()
{

}

void PropertyBrowserGuiLayer::SetPath( FS::path Path )
{

}

void PropertyBrowserGuiLayer::Render()
{
	ImGui::SetNextWindowPos( ImVec2( (float)m_PosX, (float)m_PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_Width, (float)m_Height ) );

	ImGui::Begin( m_Name.c_str() );
	if ( m_Resource != nullptr )
	{
		RenderCommon();
		ImGui::Separator();
		RenderType();
	}
	ImGui::End();
}

void PropertyBrowserGuiLayer::RenderCommon()
{
	if ( ImGui::TreeNode("Name") )
	{
		ImGui::Text( m_Resource->GetName().c_str() );
		ImGui::TreePop();
	}
	ImGui::Separator();

	if ( ImGui::TreeNode("UUID") )
	{
		ImGui::Text( m_Resource->GetUUID().c_str() );
		ImGui::TreePop();
	}
	ImGui::Separator();
	if ( ImGui::TreeNode("Type") )
	{
		std::string Typename( m_Resource->GetType()->name() );
		Typename.erase( Typename.begin(), Typename.begin()+6 );
		ImGui::Text( Typename.c_str( ) );
		ImGui::TreePop();
	}
}

void PropertyBrowserGuiLayer::RenderType()
{
	const std::type_info*& Type = m_Resource->GetType();

	if ( Type == &typeid( Scene ) ) { RenderScene(); }
	else if ( Type == &typeid( Component ) ) { RenderComponent(); }
	else if ( Type == &typeid( Camera ) ) { RenderCamera(); }
	else if ( Type == &typeid( Light ) ) { RenderLight(); }
	else if ( Type == &typeid( Mesh ) ) { RenderMesh(); }
	else if ( Type == &typeid( Script ) ) { RenderScript(); }
	else if ( Type == &typeid( Shader ) ) { RenderShader(); }
	else if ( Type == &typeid( Texture ) ) { RenderTexture(); }
}

void PropertyBrowserGuiLayer::RenderScene()
{
	Scene* TempResource = (Scene*)m_Resource;

	RenderPart< Scene, Component>( TempResource );
	RenderPart< Scene, Camera>( TempResource );
	RenderPart< Scene, Light>( TempResource );
}

void PropertyBrowserGuiLayer::RenderComponent()
{
	Component* TempResource = (Component*)m_Resource;

	RenderPart< Component, Mesh>( TempResource );
	RenderPart< Component, Script>( TempResource );
	RenderPart< Component, Shader>( TempResource );
	RenderPart< Component, Texture>( TempResource );
}

void PropertyBrowserGuiLayer::RenderCamera()
{
	Camera* TempResource = (Camera*)m_Resource;

	ImGui::Text( typeid( *TempResource ).name() );
}

void PropertyBrowserGuiLayer::RenderLight()
{
	Light* TempResource = (Light*)m_Resource;

	ImGui::Text( typeid( *TempResource ).name() );
}

void PropertyBrowserGuiLayer::RenderMesh()
{
	Mesh* TempResource = (Mesh*)m_Resource;

	ImGui::Text( typeid( *TempResource ).name() );
}

void PropertyBrowserGuiLayer::RenderScript()
{
	Script* TempResource = (Script*)m_Resource;

	ImGui::Text( typeid( *TempResource ).name() );
}

void PropertyBrowserGuiLayer::RenderShader()
{
	Shader* TempResource = (Shader*)m_Resource;

	ImGui::Text( typeid( *TempResource ).name() );
}

void PropertyBrowserGuiLayer::RenderTexture()
{
	Texture* TempResource = (Texture*)m_Resource;

	ImGui::Text( typeid( *TempResource ).name() );
}

void PropertyBrowserGuiLayer::RenderXMFLOAT2( XMFLOAT2& Info )
{

}

void PropertyBrowserGuiLayer::RenderXMFLOAT3( XMFLOAT3& Info )
{

}

void PropertyBrowserGuiLayer::RenderXMFLOAT4( XMFLOAT4& Info )
{

}

void PropertyBrowserGuiLAyer::RenderColor( XMFLOAT4& Info )
{

}

void PropertyBrowserGuiLayer::SetResource( IResource* Resource ) { m_Resource = Resource; }

void PropertyBrowserGuiLayer::Clear() { m_Resource = nullptr; }