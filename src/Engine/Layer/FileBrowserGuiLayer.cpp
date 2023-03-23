#include "FileBrowserGuiLayer.h"

FileBrowserGuiLayer::FileBrowserGuiLayer()
{
	m_Name = "FileBrowser";
	m_Rendering = false;
	m_Columns = 3;
}

FileBrowserGuiLayer::~FileBrowserGuiLayer()
{
	Destroy();
}

void FileBrowserGuiLayer::Init( int PosX, int PosY, int Width, int Height )
{
	m_PosX = PosX;
	m_PosY = PosY;
	m_Width = Width;
	m_Height = Height;
}

void FileBrowserGuiLayer::Destroy() {}

void FileBrowserGuiLayer::Update()
{
	m_Directories.clear();
	m_Files.clear();

	m_Directories = FileDialog::GetDirectories( m_MainPath );
	m_Files = FileDialog::GetFiles( m_Filter, m_MainPath );

	m_ColumnElement = (int)( ( m_Files.size() + 1 )/ m_Columns ) + 1;
}

void FileBrowserGuiLayer::Render()
{
	ImGui::SetNextWindowPos( ImVec2( (float)m_PosX, (float)m_PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_Width, (float)m_Height ) );

	ImGui::Begin( m_Name.c_str(), &m_Rendering );

	RenderDirectory();

	ImGui::SameLine();

	RenderFilter();

	RenderFolderInFiles( m_Extension );

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	if ( m_Mode ) RenderSelectedFiles();

	ImGui::End();

	if ( m_CurrentPath != m_MainPath )
	{
		m_MainPath = m_CurrentPath;
		Update();
	}
}

void FileBrowserGuiLayer::Begin( IBrowserGuiLayer* Layer, std::string Extension, bool Mode )
{
	m_MainPath = FS::current_path();
	m_CurrentPath = m_MainPath;
	memset( m_Temp, 0, MAX_LENGTH );

	m_Filter.clear();
	m_SelectedFiles.clear();

	m_RequringPathLayer = Layer;
	m_Extension = Extension;
	m_Mode = Mode;

	Update();

	m_Rendering = true;
}

void FileBrowserGuiLayer::SetPath( FS::path Path )
{
	m_RequringPathLayer->SetPath( Path );
}

void FileBrowserGuiLayer::End()
{
	m_Rendering = false;
}

FileBrowserGuiLayer& FileBrowserGuiLayer::Get() { return m_FileBrowserGuiLayer; }

void FileBrowserGuiLayer::RenderDirectory()
{
	ImGui::Text("Directory :");
	ImGui::SameLine();
	ImGui::SetNextItemWidth( 200.0f );
	if ( ImGui::BeginCombo("##Directory", FileDialog::GetFileName( m_Directories[0] ).c_str() ) )
	{
		for ( auto Path : m_Directories )
		{
			if ( ImGui::Selectable( FileDialog::GetFileName( Path ).c_str() ) )
			{
				m_CurrentPath = Path;
			}
		}
		ImGui::EndCombo();
	}
}

void FileBrowserGuiLayer::RenderFilter()
{
	ImGui::SetNextItemWidth( 300.0f );
	ImGui::InputText("##Input", m_Temp, MAX_LENGTH );

	ImGui::SameLine();

	if ( m_Mode )
	{
		if ( ImGui::Button("Filter") )
		{
			m_Filter = m_Temp;
			Update();
		}
	}
	else
	{
		if ( ImGui::Button("Save") )
		{
			m_Filter = m_Temp;
		}
	}

	ImGui::SameLine( m_Width - ImGui::CalcTextSize("Done").x - 24);

	if ( ImGui::Button("Done") )
	{
		if ( m_Mode )
		{
			if ( !m_SelectedFiles.empty() )
			{
				for ( auto Path : m_SelectedFiles )
				{
					if ( FileDialog::CheckFilter( m_Extension, FileDialog::GetFileName( Path ) ) )
					{
						m_Rendering = false;
						SetPath( FS::relative( Path, FS::current_path() ) );
					}
				}
				End();
			}
		}
		else
		{
			if ( FileDialog::CheckFilter( m_Extension, m_Filter ) )
			{
				m_Rendering = false;
				m_Filter = m_CurrentPath.generic_string() + "/" + m_Filter;
				SetPath( FS::relative( m_Filter, FS::current_path() ) );
			}
		}
	}
}

void FileBrowserGuiLayer::RenderFolderInFiles( std::string Extension )
{
	ImGui::SetNextWindowContentSize( ImVec2( (float)m_Width - 100, 0 ) );
	ImGui::BeginChild("##Files", ImVec2( 0, (float)(m_Height - 250 ) ), true, ImGuiWindowFlags_HorizontalScrollbar );

	ImGui::Columns( m_Columns );
	int Count = 1;
	ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 255, 255, 50, 255 ) );
	if ( ImGui::Selectable( "..(Up)" ) && m_MainPath.generic_string() != "C:" )
	{
		FileDialog::PathGoUp( m_CurrentPath );
	}
	ImGui::PopStyleColor();

	for ( auto Path : m_Files )
	{
		if ( FileDialog::IsPathFile( Path ) ) ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 255, 50, 255, 255 ) );
		else ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 50, 255, 255, 255 ) );


		if ( ImGui::Selectable( FileDialog::GetFileName( Path ).c_str() ) )
		{
			if ( !FileDialog::IsPathFile( Path ) )
			{
				FileDialog::PathGoDown( Path, m_CurrentPath );
			}
			else
			{
				Path = FS::relative( Path, FS::current_path() );
				if ( FileDialog::CheckFilter( Extension, FileDialog::GetFileName( Path ) ) )
				{
					m_SelectedFiles.insert( Path );
				}
			}
		}
		ImGui::PopStyleColor();

		if ( ( Count ) % ( m_ColumnElement ) == m_ColumnElement - 1 )
		{
			ImGui::NextColumn();
		}
		Count++;
	}

	ImGui::EndChild();
}

void FileBrowserGuiLayer::RenderSelectedFiles()
{
	ImGui::Text("Selected Files :");
	ImGui::SetNextWindowContentSize( ImVec2( (float)m_Width - 100, 0 ) );
	ImGui::BeginChild("##SelectedFiles", ImVec2( 0.0f, 150.0f ), true, ImGuiWindowFlags_HorizontalScrollbar );

	for ( auto Path : m_SelectedFiles )
	{
		ImGui::Text("%s", FileDialog::GetFileName( Path ).c_str() );
	}
	ImGui::EndChild();
}

FileBrowserGuiLayer FileBrowserGuiLayer::m_FileBrowserGuiLayer;