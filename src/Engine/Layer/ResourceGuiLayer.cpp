#include "ResourceGuiLayer.h"

ResourceGuiLayer::ResourceGuiLayer()
{
	m_Name = "Resource";
	m_Rendering = true;

	memset( m_NewResource, 0, MAX_LENGTH );
	m_SelectedType = 0;
}

ResourceGuiLayer::ResourceGuiLayer( const ResourceGuiLayer& Other )
{
	m_PosX = Other.m_PosX;
	m_PosY = Other.m_PosY;
	m_Width = Other.m_Width;
	m_Height = Other.m_Height;
	m_Rendering = Other.m_Rendering;
}

ResourceGuiLayer::~ResourceGuiLayer()
{
	Destroy();
}

void ResourceGuiLayer::Init( int PosX, int PosY, int Width, int Height )
{
	m_PosX = PosX;
	m_PosY = PosY;
	m_Width = Width;
	m_Height = Height;
}

void ResourceGuiLayer::Destroy()
{

}

void ResourceGuiLayer::Update()
{

}

void ResourceGuiLayer::Render()
{
	ImGui::SetNextWindowPos( ImVec2( (float)m_PosX, (float)m_PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_Width, (float)m_Height ) );

	ImGui::Begin( m_Name.c_str() );
	{
		RenderTypeCategory();
		RenderAddCategory();
	}
	ImGui::End();
}

void ResourceGuiLayer::RenderTypeCategory()
{
	if ( ImGui::TreeNode("Main") )
	{
		RenderTree<Scene>();
		RenderTree<Camera>();
		RenderTree<Component>();
		RenderTree<Light>();
		ImGui::TreePop();
	}
	if ( ImGui::TreeNode("Sub") )
	{
		RenderTree<Mesh>();
		RenderTree<Script>();
		RenderTree<Shader>();
		RenderTree<Texture>();
		ImGui::TreePop();
	}
}

void ResourceGuiLayer::RenderAddCategory()
{
	if ( ImGui::TreeNode("Add New") )
	{
		ImGui::Text("Name");
		ImGui::SameLine();
		ImGui::InputText( "##Name", m_NewResource, MAX_LENGTH );

		ImGui::Text("Type");
		ImGui::SameLine();

		const char* Items[] = {"None","Scene","Component","Camera","Light","Mesh","Script","Shader","Texture"};
		if ( ImGui::BeginCombo("##State", Items[m_SelectedType] ) )
		{
			for ( int I = 0; I < IM_ARRAYSIZE( Items ); I++ )
			{
				if ( ImGui::Selectable( Items[ I ] ) )	m_SelectedType = I;
			}
			ImGui::EndCombo();
		}

		if ( m_SelectedType != 0 && strlen( m_NewResource ) != 0 )
		{
			if ( ImGui::Button("Create") )
			{
				if ( Items[ m_SelectedType ] == Items[1] ) { ResourceManager::CreateResource<Scene>(m_NewResource);}
				else if ( Items[ m_SelectedType ] == Items[2] ) { ResourceManager::CreateResource<Component>(m_NewResource);}
				else if ( Items[ m_SelectedType ] == Items[3] ) { ResourceManager::CreateResource<Camera>(m_NewResource);}
				else if ( Items[ m_SelectedType ] == Items[4] ) { ResourceManager::CreateResource<Light>(m_NewResource);}
				else if ( Items[ m_SelectedType ] == Items[5] ) { ResourceManager::CreateResource<Mesh>(m_NewResource);}
				else if ( Items[ m_SelectedType ] == Items[6] ) { ResourceManager::CreateResource<Script>(m_NewResource);}
				else if ( Items[ m_SelectedType ] == Items[7] ) { ResourceManager::CreateResource<Shader>(m_NewResource);}
				else if ( Items[ m_SelectedType ] == Items[8] ) { ResourceManager::CreateResource<Texture>(m_NewResource);}
				m_SelectedType = 0;
				memset( m_NewResource, 0, MAX_LENGTH );
			}
		}
		ImGui::TreePop();
	}
}

void ResourceGuiLayer::SetPropertyBrowserGuiLayer( PropertyBrowserGuiLayer* PropertyBrowserGui )
{
	m_PropertyBrowserGuiLayer = PropertyBrowserGui;
}
