#include "Readtext.h"
#include <fstream>
#include <iostream>

std::ifstream READTEXT::MyFile;

char READTEXT::FileName[ MAXLENGTH ] = "";
bool READTEXT::Play = true;

FS::path READTEXT::MainPath = FS::current_path();
FS::path READTEXT::TempPath;

void READTEXT::FileLoad( FS::path Path )
{
	MyFile.open( Path.generic_string().c_str(), std::ifstream::in );
}

void READTEXT::FileRead( FS::path Path )
{
	std::string Content;

	system("cls");
	LOG_INFO(" Start to Read File ");
	LOG_INFO("");
	while( std::getline( MyFile, Content ) )
	{
		LOG_INFO("%s", Content.c_str() );
		Sleep( 10 );
	}
	LOG_INFO("");
	LOG_INFO(" End to Read File ");
	Sleep( 500 );
}

void READTEXT::FileCopy( FS::path OriginPath, FS::path NewPath, int OriginFileHandling )
{
	if ( CheckExisted( OriginPath) )
	{
		switch ( OriginFileHandling )
		{
			case ALL_ORIGIN_FILE :
				{
					FS::copy( OriginPath, NewPath );
					break;
				}
			case SKIP_ORIGIN_FILE :
				{
					FS::copy( OriginPath, NewPath, FS::copy_options::skip_existing );
					break;
				}
			case OVERWRITE_ORIGIN_FILE :
				{
					FS::copy( OriginPath, NewPath, FS::copy_options::overwrite_existing );
					break;
				}
			case UPDATE_ORIGIN_FILE :
				{
					FS::copy( OriginPath, NewPath, FS::copy_options::update_existing );
					break;
				}
		}
	}
}

void READTEXT::FileMove( FS::path OriginPath, FS::path NewPath )
{
	READTEXT::FileCopy( OriginPath, NewPath, ALL_ORIGIN_FILE );
	READTEXT::FileDelete( OriginPath );
}

void READTEXT::FileDelete( FS::path Path )
{
	if ( CheckExisted( Path ) )
	{
		FS::remove( Path );
	}
}

bool READTEXT::CheckExisted( FS::path Path )
{
	if ( FS::exists( Path ) )
	{
		LOG_INFO(" %s is existed ", Path.generic_string().c_str() );
		return true;
	}
	else
	{
		LOG_ERROR(" %s is not existed ", Path.generic_string().c_str() );
		return false;
	}
}

PATHTYPE READTEXT::CheckPathType( FS::path Path )
{
	if ( FS::is_regular_file( Path ) )
	{
		LOG_INFO(" %s is file ", GetFileName( Path.generic_string().c_str() ) );
		return FILEPATH;
	}
	else if ( FS::is_directory( Path ) )
	{
		LOG_INFO(" %s is directory ", GetFileName( Path.generic_string().c_str() ) );
		return DIRECTORYPATH;
	}
	else
	{
		LOG_INFO(" %s is NULL ", GetFileName( Path.generic_string().c_str() ) );
		return NULLPATH;
	}
}

void READTEXT::CheckFileType( FS::path Path )
{

}

void READTEXT::TypeWindow( FS::path Path )
{

}

void READTEXT::TypeObject( FS::path Path )
{

}

void READTEXT::TypeStory( FS::path Path )
{

}

void READTEXT::TypeBrowser( FS::path Path )
{

}

void READTEXT::PrintCurrentPath()
{
	LOG_INFO(" Current Path : %s", FS::current_path().generic_string().c_str() );
}

void READTEXT::PrintRelativePath( FS::path Path )
{
	LOG_INFO(" Relative Path : %s", Path.relative_path().generic_string().c_str() );
}

void READTEXT::PrintAbsolutePath( FS::path Path )
{
	LOG_INFO(" Absolute Path : %s", FS::absolute( Path ).generic_string().c_str() );
}

void READTEXT::PrintCanonicalAbsoluatePath( FS::path Path )
{
	LOG_INFO(" Canonical Absolute Path : %s", FS::canonical( Path ).generic_string().c_str() );
}

void READTEXT::PrintFilesinDirectory( FS::path Path )
{
	FS::directory_iterator ITR( Path );

	LOG_INFO(" Main Path : %s ", Path.generic_string().c_str() );
	while ( ITR != FS::end( ITR ) )
	{
		const FS::directory_entry& ENTRY = *ITR;
		LOG_INFO(" Path: %s", GetFileName( ENTRY.path().generic_string().c_str() ) );
		ITR++;
		Sleep( 100 );
	}
}

void READTEXT::PrintAllFilesinDirectory( FS::path Path )
{
	FS::recursive_directory_iterator ITR( Path );

	LOG_INFO(" Main Path : %s ", Path.generic_string().c_str() );
	while ( ITR != FS::end( ITR ) )
	{
		const FS::directory_entry& ENTRY = *ITR;
		LOG_INFO(" Path: %s", GetFileName( ENTRY.path().generic_string().c_str() ) );
		ITR++;
		Sleep( 100 );
	}
}

void READTEXT::CreateDirectory( FS::path Path )
{
	if ( CheckExisted( Path ) )
	{
		LOG_ERROR(" %s is Already Existed ", Path.generic_string().c_str() );
		return;
	}
	else
	{
		LOG_INFO(" Create ");
		FS::create_directory( Path );
	}
}

void READTEXT::CreateAllDirectory( FS::path Path )
{
	if ( CheckExisted( Path ) )
	{
		LOG_ERROR(" %s is Already Existed ", Path.generic_string().c_str() );
		return;
	}
	else
	{
		LOG_INFO(" Create ");
		FS::create_directories( Path );
	}
}

void READTEXT::PathGo2Up( FS::path& Path )
{
	std::string StrPath = Path.generic_string();
	std::string PathName = "/";
	PathName += GetFileName( StrPath.c_str() );
	StrPath.erase( StrPath.end() - PathName.length() , StrPath.end() );
	Path = StrPath;
}

void READTEXT::PathGo2Down( FS::path& Path, char* DownPath, bool Read )
{
	std::string StrPath = Path.generic_string();
	StrPath += "/";
	StrPath += DownPath;
	PATHTYPE pathtype = CheckPathType( (FS::path)StrPath );
	switch( pathtype )
	{
	case NULLPATH :
		LOG_ERROR(" Error Path ");
		break;
	case FILEPATH :
		if ( Read )
		{
			Path = StrPath;
		}
		else
		{
			LOG_INFO(" Select Other Path ");
		}
		break;
	case DIRECTORYPATH :
		Path = StrPath;
		break;
	default :
		break;
	}
}

char* READTEXT::GetFileName( const char* Path )
{
	const char* ptr = NULL;

	ptr = strrchr( Path, '/' );

	if ( ptr == NULL )
	{
		strcpy( FileName, Path );
	}
	else
	{
		strcpy( FileName, ptr + 1 );
	}
	return FileName;
}

void READTEXT::Frame()
{
	PrintFilesinDirectory( MainPath );

	LOG_INFO(" 0 : End Browser ");
	LOG_INFO(" 1 : Up Directory ");
	LOG_INFO(" 2 : Down Directory ");
	LOG_INFO(" 3 : Read File ");

	int Value;
	printf(" Input Value ");
	scanf("%d", &Value );

	switch( Value )
	{
		case 0 :
			Play = false;
			break;
		case 1:
			PathGo2Up( MainPath );
			break;
		case 2:
			printf(" Select File ");
			scanf("%s", &FileName );
			PathGo2Down( MainPath, FileName, false );
			break;
		case 3:
			printf(" Select File ");
			scanf("%s", &FileName );
			TempPath= MainPath;
			READTEXT::PathGo2Down( MainPath, FileName, true );
			READTEXT::FileLoad( MainPath );
			READTEXT::FileRead( MainPath );
			MainPath = TempPath;
			break;
		default :
			break;
	}

	Sleep ( 200 );
}