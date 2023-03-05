#include "DIFileBrowser.h"
#include "SUBLog.h"
#include <sstream>

DIFILEBROWSER::DIFILEBROWSER()
{
	InitPointer();
	m_BrowserRect = new DXRECTWINDOW;
	m_BrowserRect->Name = "Browser";
	m_BrowserRect->PosX = 400;
	m_BrowserRect->PosY = 200;
	m_BrowserRect->Width = 840;
	m_BrowserRect->Height = 320;

	m_ColumnElement = 13;
}

DIFILEBROWSER::~DIFILEBROWSER()
{

}

void DIFILEBROWSER::InitPointer()
{

}

bool DIFILEBROWSER::Frame()
{
	m_MainPath = FS::current_path();
	memset( m_FileName, 0, MAXLENGTH );
	memset( m_FileFilter, 0, MAXLENGTH );
	memset( m_TempFile, 0, MAXLENGTH );
	memset( m_ResultPath, 0, MAXLENGTH );
	return true;
}

void DIFILEBROWSER::Render( bool& Rendering )
{
	ImGui::SetNextWindowPos( ImVec2( (float)m_BrowserRect->PosX, (float)m_BrowserRect->PosY ) );
	ImGui::SetNextWindowSize( ImVec2( (float)m_BrowserRect->Width, (float)m_BrowserRect->Height ));

	ImGui::Begin(" FileBrowser ", NULL );
	std::vector<std::string> Data = split( m_MainPath.generic_string(), '/' );

	std::string Temp;
	for ( int I = 0; I < Data.size(); I++ )
	{
		if ( ImGui::Button(Data[I].c_str() ) )
		{
			for ( int J = 0; J <= I ; J++ )
			{
				Temp += Data[ J ];
				if ( J != I )
				{
					Temp += "/";
				}
			}
			m_MainPath = Temp;
		}

		if ( I < Data.size() - 1 )
		{
			ImGui::SameLine();
			ImGui::Text("/");
			ImGui::SameLine();
		}
	}

	ImGui::SetNextWindowContentSize( ImVec2( m_BrowserRect->Width, 0 ) );
	ImGui::BeginChild( "##ScrollRegin", ImVec2( 0, m_BrowserRect->Height - 80 ), true, ImGuiWindowFlags_HorizontalScrollbar );
	PrintAllFilesInDirectory( m_MainPath );
	ImGui::EndChild();
	ImGui::Spacing();
	ImGui::InputText( "##Filter", m_TempFile, MAXLENGTH );
	ImGui::SameLine();
	if ( ImGui::Button("Set") )
	{
		strcpy( m_FileFilter, m_TempFile );
	}
	ImGui::SameLine( m_BrowserRect->Width - ImGui::CalcTextSize("Done").x - 24 );
	if ( ImGui::Button("End") )
	{
		Rendering = false;
	}
	if ( m_ResultPath != nullptr )
	{
		ImGui::Text(" Selected File : %s", m_ResultPath );
	}
	ImGui::End();
}

void DIFILEBROWSER::PrintAllFilesInDirectory( FS::path Path )
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
	ImGui::Columns( floor( FileData.size() / m_ColumnElement ) + 1 );

	for ( int I = 0; I <= FileData.size(); I++ )
	{
		if ( I == 0 )
		{
			ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 255, 255, 0, 255 ) );
			if ( ImGui::Selectable( "..(Up)" ) && Path.generic_string() != "C:/" )
			{
				PathGo2Up( m_MainPath );
			}
			ImGui::PopStyleColor();
		}
		else
		{
			switch( FileData[ I - 1 ].Type )
			{
				case PATHTYPE::FOLDER:
					ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 0, 255, 0, 255 ) );
						break;
				case PATHTYPE::FILE :
					ImGui::PushStyleColor( ImGuiCol_Text, IM_COL32( 255, 255, 255, 255 ) );
						break;
			};
			GetFileName( FileData[ I - 1].Path.generic_string().c_str() );

			if ( ImGui::Selectable( m_FileName ) )
			{
				if ( FileData[ I - 1 ].Type == PATHTYPE::FOLDER )
				{
					PathGo2Down( FileData[ I - 1 ].Path, m_MainPath );
				}
				else if ( FileData[ I - 1].Type == PATHTYPE::FILE )
				{
					strcpy( m_ResultPath, FileData[ I - 1 ].Path.generic_string().c_str() );
				}
			}
			ImGui::PopStyleColor();

			if ( ( I + 1 ) % m_ColumnElement == 0 )
			{
				ImGui::NextColumn();
			}
		}
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

using namespace std;

vector<string> DIFILEBROWSER::split(string input, char delimiter)
{
    vector<string> answer;
    stringstream ss(input);
    string temp;

    while (getline(ss, temp, delimiter)) {
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