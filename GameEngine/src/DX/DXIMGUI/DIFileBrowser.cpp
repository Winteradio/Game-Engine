#include "DIFileBrowser.h"
#include "SUBLog.h"
#include <sstream>

DIFILEBROWSER::DIFILEBROWSER()
{
	InitPointer();
}

DIFILEBROWSER::~DIFILEBROWSER() {}

bool DIFILEBROWSER::Init()
{
	m_BrowserRect = new DXRECTWINDOW;
	m_BrowserRect->Name = "Browser";
	m_BrowserRect->PosX = 400;
	m_BrowserRect->PosY = 200;
	m_BrowserRect->Width = 840;
	m_BrowserRect->Height = 340;


	m_OriginPath = FS::current_path();
	m_ColumnElement = 13;

	memset( m_FileName, 0, MAXLENGTH );
	memset( m_FileFilter, 0, MAXLENGTH );
	memset( m_TempFile, 0, MAXLENGTH );
	memset( m_ResultPath, 0, MAXLENGTH );

	return true;
}

bool DIFILEBROWSER::Frame()
{
	m_MainPath = FS::current_path();
	return true;
}

void DIFILEBROWSER::Render( bool& Rendering )
{
	ImGui::SetNextWindowPos( ImVec2( (float)m_BrowserRect->PosX, (float)m_BrowserRect->PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_BrowserRect->Width, (float)m_BrowserRect->Height ));

	ImGui::Begin(" FileBrowser ", NULL );

	RenderFolderButton( m_MainPath );
	RenderFilesSelectable( m_MainPath, CheckFilesbyFilter( m_MainPath ) );
	RenderFileFilter();
	RenderEndButton( Rendering );
	CheckResult();

	ImGui::End();
}

void DIFILEBROWSER::Release()
{
	delete m_BrowserRect;

	InitPointer();
}

void DIFILEBROWSER::InitPointer()
{
	m_BrowserRect = nullptr;
}

void DIFILEBROWSER::RenderFolderButton( FS::path Path )
{
	m_FolderData = Split( Path.generic_string(), '/' );

	std::string TempPath;

	for ( int I = 0; I < m_FolderData.size(); I++ )
	{
		if ( ImGui::Button( m_FolderData[ I ].c_str() ) )
		{
			for ( int J = 0; J <= I ; J++ )
 	 		{
				TempPath += m_FolderData[ J ];
				if ( J != I )
				{
		 	 		TempPath += "/";
				}
			}
			m_MainPath = TempPath;
		}

		if ( I < m_FolderData.size() - 1 )
		{
			ImGui::SameLine();
		 	ImGui::Text("/");
			ImGui::SameLine();
		}
	}
}

void DIFILEBROWSER::RenderFileFilter()
{
	ImGui::Spacing();

	ImGui::InputText( "##Filter", m_TempFile, MAXLENGTH );

	ImGui::SameLine();

	if ( ImGui::Button("Set") )
	{
		strcpy( m_FileFilter, m_TempFile );
	}
}

void DIFILEBROWSER::RenderFilesSelectable( FS::path Path, std::vector<FILEDATA> FileData )
{
	ImGui::SetNextWindowContentSize( ImVec2( m_BrowserRect->Width, 0 ) );
	ImGui::BeginChild( "##ScrollRegin", ImVec2( 0, m_BrowserRect->Height - 100 ), true, ImGuiWindowFlags_HorizontalScrollbar );

	ImGui::Columns( (int)floor( FileData.size() / m_ColumnElement ) + 1 );

	for ( int I = 0; I <= FileData.size(); I++ )
	{
		if ( I == 0 )
		{
			ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 255, 255, 0, 255 ) );
			if ( ImGui::Selectable( "..(Up)" ) && Path.generic_string() != "C:" )
			{
				PathGo2Up( m_MainPath );
			}
			ImGui::PopStyleColor();
		}
		else
		{
			GetFileName( FileData[ I - 1].Path.generic_string().c_str() );

			switch( FileData[ I - 1 ].Type )
			{
				case PATHTYPE::FOLDER:
					ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 0, 255, 0, 255 ) );
					if ( ImGui::Selectable ( m_FileName ) )
					{
						PathGo2Down( FileData[ I - 1 ].Path, m_MainPath );
					}
					ImGui::PopStyleColor();
					break;
				case PATHTYPE::FILE :
					ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 255, 255, 255, 255 ) );
					if ( ImGui::Selectable( m_FileName ) )
					{
						strcpy( m_ResultPath, FileData[ I - 1 ].Path.generic_string().c_str() );
					}
					ImGui::PopStyleColor();
					break;
			};

			if ( ( I + 1 ) % m_ColumnElement == 0 )
			{
				ImGui::NextColumn();
			}
		}
	}

	ImGui::EndChild();
}

void DIFILEBROWSER::RenderEndButton( bool& Rendering )
{
	ImGui::SameLine( m_BrowserRect->Width - ImGui::CalcTextSize("Done").x - 24 );
	if ( ImGui::Button("End") )
	{
		Rendering = false;
	}
}

void DIFILEBROWSER::PathGo2Up( FS::path& Path )
{
	std::string StrPath = Path.generic_string();
	std::string PathName ="/";
	GetFileName( StrPath.c_str() );
	PathName += m_FileName;
	StrPath.erase( StrPath.end() - PathName.length(), StrPath.end() );
	Path = StrPath;
}

void DIFILEBROWSER::PathGo2Down( FS::path TempPath, FS::path& MainPath )
{
	MainPath = TempPath;
}

void DIFILEBROWSER::CheckResult()
{
	if ( m_ResultPath != nullptr )
	{
		ImGui::Text(" Selected File : %s", m_ResultPath );
	}
}

std::vector<FILEDATA> DIFILEBROWSER::CheckFilesbyFilter( FS::path Path )
{
	if ( Path.generic_string() == "C:" )
	{
		std::string Temp = Path.generic_string() + "/";
		Path = Temp;
	}

	FS::directory_iterator ITR( Path );

	int FileCount = 0;
	std::vector<FILEDATA> FileData;
	FILEDATA Temp;
	while( ITR != FS::end( ITR ) )
	{
		if ( m_FileFilter[0] != '0' )
		{
			const FS::directory_entry& ENTRY = *ITR;
			GetFileName( ENTRY.path().generic_string().c_str() );
			std::string TempString( m_FileName );
			if ( TempString.find( m_FileFilter ) != std::string::npos )
			{
				FileCount++;
				Temp.Type = CheckPathType( ENTRY.path() );
				Temp.Path = ENTRY.path();
				FileData.push_back( Temp );
			}
		}
		ITR++;
	}

	sort( FileData.begin(), FileData.end(), Compare );

	return FileData;
}

bool DIFILEBROWSER::CheckExisted( FS::path Path )
{
	if ( FS::exists( Path ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

PATHTYPE DIFILEBROWSER::CheckPathType( FS::path Path )
{
	if ( FS::is_regular_file( Path ) )
	{
		return PATHTYPE::FILE;
	}
	else if ( FS::is_directory( Path ) )
	{
		return PATHTYPE::FOLDER;
	}
}

void DIFILEBROWSER::GetFileName( const char* Path )
{
	const char* ptr = nullptr;

	ptr = strrchr( Path, '/' );

	if ( ptr == nullptr )
	{
		strcpy( m_FileName, Path );
	}
	else
	{
		strcpy( m_FileName, ptr + 1 );
	}
}

char* DIFILEBROWSER::GetFilePath()
{
	FS::path Base = FS::current_path();
	if ( m_ResultPath[0] != '\0' )
	{
		std::string TempString( m_ResultPath );
		strcpy( m_ResultPath, FS::relative( TempString, m_OriginPath ).generic_string().c_str() );
		return m_ResultPath;
	}
	return nullptr;
}

std::vector<std::string> DIFILEBROWSER::Split(std::string Input, char Diameter)
{
    std::vector<std::string> answer;
    std::stringstream ss(Input);
    std::string temp;

    while (getline(ss, temp, Diameter)) {
        answer.push_back(temp);
    }

    return answer;
}

bool DIFILEBROWSER::Compare( const FILEDATA& Num1, const FILEDATA& Num2 )
{
	if ( Num1.Type != Num2.Type )
	{
		if ( Num1.Type == PATHTYPE::FOLDER )
		{
			return true;
		}
		else if ( Num1.Type == PATHTYPE::FILE )
		{
			return false;
		}
	}
	else
	{
		if ( Num1.Path.generic_string() <= Num2.Path.generic_string() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}