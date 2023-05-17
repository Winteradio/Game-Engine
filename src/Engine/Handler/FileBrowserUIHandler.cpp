#include "FileBrowserUIHandler.h"

#include "AssetHandler.h"

FileBrowserUIHandler::FileBrowserUIHandler() {}

FileBrowserUIHandler::~FileBrowserUIHandler() {}

void FileBrowserUIHandler::Init( int PosX, int PosY, int Width, int Height )
{
    m_PosX = PosX;
    m_PosY = PosY;
    m_Width = Width;
    m_Height = Height; 

    m_Rendering = false;
    m_Columns = 3;
    Log::Info(" Init - File Browser UI Handler ");
}

void FileBrowserUIHandler::Destroy()
{
    m_SelectedFiles.clear();
    m_Directories.clear();
    m_Files.clear();
}

void FileBrowserUIHandler::Update( float DeltaTime )
{
    if ( !m_Rendering ) return;

	ImGui::SetNextWindowPos( ImVec2( (float)m_PosX, (float)m_PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_Width, (float)m_Height ) );

//

    ImGui::Begin( "File Browser", &m_Rendering );

//

	ImGui::Text("Directory :");
	ImGui::SameLine();
	ImGui::SetNextItemWidth( 200.0f );
	if ( ImGui::BeginCombo("##Directory", FileHandler::GetHandle().GetFileName( m_Directories[0] ).c_str() ) )
	{
		for ( auto Path : m_Directories )
		{
			if ( ImGui::Selectable( FileHandler::GetHandle().GetFileName( Path ).c_str() ) )
			{
				m_CurrentPath = Path;
			}
		}
		ImGui::EndCombo();
	}

//

	ImGui::SameLine();

//

	ImGui::SetNextItemWidth( 300.0f );
	ImGui::InputText("##Input", m_Temp, MAX_LENGTH );

	ImGui::SameLine();

	if ( m_Mode )
	{
		if ( ImGui::Button("Filter") )
		{
			m_Filter = m_Temp;

			UpdateFile();

	        m_ColumnElement = (int)( ( m_Files.size() + 1 )/ m_Columns ) + 1;
		}
	}
	else
	{
		if ( ImGui::Button("Save") )
		{
			m_Filter = m_Temp;
		}
	}

//

	ImGui::SameLine( m_Width - ImGui::CalcTextSize("Done").x - 24);

	if ( ImGui::Button("Done") )
	{
		if ( m_Mode )
		{
			if ( !m_SelectedFiles.empty() )
			{
				for ( auto Path : m_SelectedFiles )
				{
					if ( FileHandler::GetHandle().CheckFilter( m_Extension, FileHandler::GetHandle().GetFileName( Path ) ) )
					{
						m_Rendering = false;
						AssetHandler::GetHandle().Load( Path );
					}
				}
				End();
			}
		}
		else
		{
			if ( FileHandler::GetHandle().CheckFilter( m_Extension, m_Filter ) )
			{
				m_Rendering = false;
				m_Filter = m_CurrentPath.generic_string() + "/" + m_Filter;
                AssetHandler::GetHandle().Save( m_Filter );
			}
		}
	}

//

	ImGui::SetNextWindowContentSize( ImVec2( (float)m_Width - 100, 0 ) );
	ImGui::BeginChild("##Files", ImVec2( 0, (float)(m_Height - 250 ) ), true, ImGuiWindowFlags_HorizontalScrollbar );

	ImGui::Columns( m_Columns );
	int Count = 1;
	ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 255, 255, 50, 255 ) );
	if ( ImGui::Selectable( "..(Up)" ) && m_MainPath.generic_string() != "C:" )
	{
		FileHandler::GetHandle().PathGoUp( m_CurrentPath );
	}
	ImGui::PopStyleColor();

	for ( auto Path : m_Files )
	{
		if ( FileHandler::GetHandle().IsPathFile( Path ) ) ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 255, 50, 255, 255 ) );
		else ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 50, 255, 255, 255 ) );

		if ( ImGui::Selectable( FileHandler::GetHandle().GetFileName( Path ).c_str() ) )
		{
			if ( !FileHandler::GetHandle().IsPathFile( Path ) )
			{
				FileHandler::GetHandle().PathGoDown( Path, m_CurrentPath );
			}
			else
			{
				Path = FS::relative( Path, FS::current_path() );
				if ( FileHandler::GetHandle().CheckFilter( m_Extension, FileHandler::GetHandle().GetFileName( Path ) ) )
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

//

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

//

	ImGui::Text("Selected Files :");
	ImGui::SetNextWindowContentSize( ImVec2( (float)m_Width - 100, 0 ) );
	ImGui::BeginChild("##SelectedFiles", ImVec2( 0.0f, 150.0f ), true, ImGuiWindowFlags_HorizontalScrollbar );

	for ( auto Path : m_SelectedFiles )
	{
		ImGui::Text("%s", FileHandler::GetHandle().GetFileName( Path ).c_str() );
	}
	ImGui::EndChild();

//

    ImGui::End();

    if ( m_MainPath != m_CurrentPath )
    {
        m_MainPath = m_CurrentPath;

		UpdateFile();

	    m_ColumnElement = (int)( ( m_Files.size() + 1 )/ m_Columns ) + 1;
    }
}

void FileBrowserUIHandler::UpdateFile()
{
	m_Directories.clear();
	m_Directories = FileHandler::GetHandle().UpdateDirectories( m_MainPath );

	m_Files.clear();
	m_Files = FileHandler::GetHandle().UpdateFiles( m_Filter, m_MainPath );
}

void FileBrowserUIHandler::Begin( std::string Extension, bool Mode ) 
{
    m_Extension = Extension;
    m_Mode = Mode;
    m_Rendering = true;

    m_MainPath = FS::current_path();
    m_CurrentPath = m_MainPath;

    m_Filter.clear();
    m_SelectedFiles.clear();

	UpdateFile();

	m_ColumnElement = (int)( ( m_Files.size() + 1 )/ m_Columns ) + 1;
}
void FileBrowserUIHandler::End() { m_Rendering = false; }

FileBrowserUIHandler& FileBrowserUIHandler::GetHandle() { return m_FileBrowserUIHandler; }

FileBrowserUIHandler FileBrowserUIHandler::m_FileBrowserUIHandler;