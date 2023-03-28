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

	memset( m_Temp, 0, MAX_LENGTH );
}

void PropertyBrowserGuiLayer::Destroy()
{

}

void PropertyBrowserGuiLayer::Update()
{

}

void PropertyBrowserGuiLayer::SetPath( FS::path Path )
{
	if ( m_Resource != nullptr )
	{
		( (IFileResource*)m_Resource )->SetFilePath( Path );
	}
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
	if ( ImGui::TreeNode("Common") )
	{
		ImGui::SeparatorText("Name");
		ImGui::SetNextItemWidth( 200.0f );
		ImGui::InputText("##Input", m_Temp, MAX_LENGTH );
		ImGui::SameLine();
		if ( ImGui::Button("Set") )
		{
			m_Resource->SetName( m_Temp );
		}


		ImGui::SeparatorText("UUID");
		ImGui::Text( m_Resource->GetUUID().c_str() );

		ImGui::SeparatorText("Type");
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

	if ( ImGui::TreeNode("Transform") )
	{
		RenderXMFLOAT3( "Translation", TempResource->GetTranslation() );
		RenderXMFLOAT3( "Rotation", TempResource->GetRotation() );
		RenderXMFLOAT3( "Scale", TempResource->GetScale() );
		ImGui::TreePop();
	}

	ImGui::Separator();

	if ( ImGui::TreeNode("Part") )
	{
		RenderPart< Scene, Component>( TempResource );
		RenderPart< Scene, Camera>( TempResource );
		RenderPart< Scene, Light>( TempResource );
		ImGui::TreePop();
	}
}

void PropertyBrowserGuiLayer::RenderComponent()
{
	Component* TempResource = (Component*)m_Resource;

	if ( ImGui::TreeNode("Transform") )
	{
		RenderXMFLOAT3( "Translation", TempResource->GetTranslation() );
		RenderXMFLOAT3( "Rotation", TempResource->GetRotation() );
		RenderXMFLOAT3( "Scale", TempResource->GetScale() );
		ImGui::TreePop();
	}

	ImGui::Separator();

	if ( ImGui::TreeNode("Part") )
	{
		RenderPart< Component, Mesh>( TempResource );
		RenderPart< Component, Script>( TempResource );
		if ( ImGui::TreeNode("Shader") )
		{
			RenderPart< Component, VertexShader >( TempResource );
			RenderPart<	Component, PixelShader >( TempResource );
			ImGui::TreePop();
		}
		RenderPart< Component, Texture>( TempResource );
		ImGui::TreePop();
	}
}

void PropertyBrowserGuiLayer::RenderCamera()
{
	Camera* TempResource = (Camera*)m_Resource;

	if ( ImGui::TreeNode("Transform") )
	{
		RenderXMFLOAT3( "Translation", TempResource->GetTranslation() );
		RenderXMFLOAT3( "Rotation", TempResource->GetRotation() );
		RenderXMFLOAT3( "Scale", TempResource->GetScale() );
		ImGui::TreePop();
	}
}

void PropertyBrowserGuiLayer::RenderLight()
{
	Light* TempResource = (Light*)m_Resource;

	if ( ImGui::TreeNode("Transform") )
	{
		RenderXMFLOAT3( "Translation", TempResource->GetTranslation() );
		RenderXMFLOAT3( "Rotation", TempResource->GetRotation() );
		RenderXMFLOAT3( "Scale", TempResource->GetScale() );
		ImGui::TreePop();
	}

	ImGui::Separator();

	if ( ImGui::TreeNode("Color") )
	{
		RenderColor( "AmbientColor", TempResource->GetAmbientColor() );
		RenderColor( "DiffuseColor", TempResource->GetDiffuseColor() );
		RenderColor( "SpecularColor", TempResource->GetSpecularColor() );
		RenderFloat( "SpecularPower", TempResource->GetSpecularPower() );
		ImGui::TreePop();
	}
}

void PropertyBrowserGuiLayer::RenderMesh()
{
	Mesh* TempResource = (Mesh*)m_Resource;

	if ( ImGui::TreeNode("File") )
	{
		if ( ImGui::Button("Set") )
		{
			FileBrowserGuiLayer::Get().Begin( this, "", true );
		}
		ImGui::Text("Path : ");
		ImGui::SameLine();
		ImGui::Text( FileDialog::GetFileName( TempResource->GetFilePath() ).c_str() );

		ImGui::Text("Name : ");
		ImGui::SameLine();
		ImGui::Text( FileDialog::GetFileName( TempResource->GetFilePath() ).c_str() );

		ImGui::TreePop();
	}
}

void PropertyBrowserGuiLayer::RenderScript()
{
	Script* TempResource = (Script*)m_Resource;

	if ( ImGui::TreeNode("File") )
	{
		ImGui::Text("Path : ");
		ImGui::SameLine();
		ImGui::Text( FileDialog::GetFileName( TempResource->GetFilePath() ).c_str() );

		ImGui::Text("Name : ");
		ImGui::SameLine();
		ImGui::Text( FileDialog::GetFileName( TempResource->GetFilePath() ).c_str() );

		if ( ImGui::Button("Set") )
		{
			FileBrowserGuiLayer::Get().Begin( this, "", true );
		}

		ImGui::TreePop();
	}
}

void PropertyBrowserGuiLayer::RenderShader()
{
	Shader* TempResource = (Shader*)m_Resource;

	if ( ImGui::TreeNode("File") )
	{
		ImGui::Text("Path : ");
		ImGui::SameLine();
		ImGui::Text( FileDialog::GetFileName( TempResource->GetFilePath() ).c_str() );

		ImGui::Text("Name : ");
		ImGui::SameLine();
		ImGui::Text( FileDialog::GetFileName( TempResource->GetFilePath() ).c_str() );

		if ( ImGui::Button("Set") )
		{
			FileBrowserGuiLayer::Get().Begin( this, ".hlsl", true );
		}

		ImGui::TreePop();
	}
}

void PropertyBrowserGuiLayer::RenderTexture()
{
	Texture* TempResource = (Texture*)m_Resource;

	if ( ImGui::TreeNode("File") )
	{
		ImGui::SeparatorText( "Path" );
		ImGui::Text( TempResource->GetFilePath().generic_string().c_str() );

		ImGui::SeparatorText( "Name" );
		ImGui::Text( FileDialog::GetFileName( TempResource->GetFilePath() ).c_str() );

		if ( ImGui::Button("Set") )
		{
			FileBrowserGuiLayer::Get().Begin( this, "", true );
		}
		ImGui::TreePop();
	}
}

void PropertyBrowserGuiLayer::RenderFloat( std::string Name, float& Info, float ResetValue )
{
	ImGui::SeparatorText( Name.c_str() );

	ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );

	if ( ImGui::Button("Reset") ) Info = ResetValue;

	ImGui::SameLine();

	ImGui::PushID( ( Name ).c_str() );
	ImGui::DragFloat("##X", &Info, 0.1f );
	ImGui::PopID();
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();
}

void PropertyBrowserGuiLayer::RenderXMFLOAT2( std::string Name, XMFLOAT2& Info, float ResetValue )
{
	ImGui::SeparatorText( Name.c_str() );

	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );

	if ( ImGui::Button("X") ) Info.x = ResetValue;

	ImGui::SameLine();

	ImGui::PushID( ( Name + "X" ).c_str() );
	ImGui::DragFloat("##X", &Info.x, 0.1f );
	ImGui::PopID();
	ImGui::PopItemWidth();
	ImGui::SameLine();

	if ( ImGui::Button("Y") ) Info.y = ResetValue;

	ImGui::SameLine();
	ImGui::PushID( ( Name + "Y" ).c_str() );
	ImGui::DragFloat("##Y", &Info.y, 0.1f );
	ImGui::PopID();
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();
}

void PropertyBrowserGuiLayer::RenderXMFLOAT3( std::string Name, XMFLOAT3& Info, float ResetValue )
{
	ImGui::SeparatorText( Name.c_str() );

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );

	if ( ImGui::Button("X") ) Info.x = ResetValue;

	ImGui::SameLine();
	ImGui::PushID( ( Name + "X" ).c_str() );
	ImGui::DragFloat("##X", &Info.x, 0.1f );
	ImGui::PopID();
	ImGui::PopItemWidth();
	ImGui::SameLine();

	if ( ImGui::Button("Y") ) Info.y = ResetValue;

	ImGui::SameLine();
	ImGui::PushID( ( Name + "Y" ).c_str() );
	ImGui::DragFloat("##Y", &Info.y, 0.1f );
	ImGui::PopID();
	ImGui::PopItemWidth();
	ImGui::SameLine();

	if ( ImGui::Button("Z") ) Info.z = ResetValue;

	ImGui::SameLine();
	ImGui::PushID( ( Name + "Z" ).c_str() );
	ImGui::DragFloat("##Z", &Info.z, 0.1f );
	ImGui::PopID();
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();
}

void PropertyBrowserGuiLayer::RenderXMFLOAT4( std::string Name, XMFLOAT4& Info, float ResetValue )
{
	ImGui::SeparatorText( Name.c_str() );

	ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );

	if ( ImGui::Button("X") ) Info.x = ResetValue;

	ImGui::SameLine();
	ImGui::PushID( ( Name + "X" ).c_str() );
	ImGui::DragFloat("##X", &Info.x, 0.1f );
	ImGui::PopID();
	ImGui::PopItemWidth();
	ImGui::SameLine();

	if ( ImGui::Button("Y") ) Info.y = ResetValue;

	ImGui::SameLine();
	ImGui::PushID( ( Name + "Y" ).c_str() );
	ImGui::DragFloat("##Y", &Info.y, 0.1f );
	ImGui::PopID();
	ImGui::PopItemWidth();
	ImGui::SameLine();

	if ( ImGui::Button("Z") ) Info.z = ResetValue;

	ImGui::SameLine();
	ImGui::PushID( ( Name + "Z" ).c_str() );
	ImGui::DragFloat("##Z", &Info.z, 0.1f );
	ImGui::PopID();
	ImGui::PopItemWidth();
	ImGui::SameLine();

	if ( ImGui::Button("W") ) Info.z = ResetValue;

	ImGui::SameLine();
	ImGui::PushID( ( Name + "W" ).c_str() );
	ImGui::DragFloat("##W", &Info.w, 0.1f );
	ImGui::PopID();
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();
}

void PropertyBrowserGuiLayer::RenderColor( std::string Name, XMFLOAT4& Info, float ResetValue )
{
	ImGui::SeparatorText( Name.c_str() );

	static ImVec4 Color = ImVec4( Info.x, Info.y, Info.z, Info.w );
	ImGui::ColorEdit4("##2f", (float*)&Color, ImGuiColorEditFlags_Float );
	Info.x = Color.x;
	Info.y = Color.y;
	Info.z = Color.z;
	Info.w = Color.w;
}

void PropertyBrowserGuiLayer::SetResource( IResource* Resource )
{
	m_Resource = Resource;
	strcpy( m_Temp, m_Resource->GetName().c_str() );
}

void PropertyBrowserGuiLayer::Clear() { m_Resource = nullptr; }