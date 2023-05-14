#include "FileHandler.h"

FileHandler::FileHandler()
{

}

FileHandler::~FileHandler()
{

}

void FileHandler::Init()
{
    Log::Info(" Init - File Handler ");
}

void FileHandler::Destroy()
{
    Log::Info(" Destroy - File Handler ");
}

void FileHandler::PathGoUp( FS::path& Path )
{
	std::string stringPath = Path.generic_string();
	std::string ErasedPath = "/" + GetFileName( Path );
	stringPath.erase( stringPath.end() - ErasedPath.length(), stringPath.end() );

	Path = stringPath;
}

void FileHandler::PathGoDown( FS::path FolderPath, FS::path& MainPath )
{
	MainPath = FolderPath;
}

bool FileHandler::CheckFilter( std::string Filter, FS::path Path )
{
	if ( Filter.empty() )
	{
		return true;
	}
	else
	{
		if ( Path.generic_string().find( Filter ) != std::string::npos ) return true;
		else return false;
	}
}

bool FileHandler::IsPathFile( FS::path Path )
{
	if ( FS::is_directory( Path ) )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Compare( FS::path One, FS::path Other )
{
	std::string stringOne = One.generic_string();
	std::string stringOther = Other.generic_string();
	if ( FS::is_regular_file( One ) && FS::is_regular_file( Other ) )
	{
		return stringOne < stringOther;
	}
	else if ( FS::is_directory( One ) && FS::is_directory( Other ) )
	{
		return stringOne < stringOther;
	}
	else if ( FS::is_regular_file( One ) && FS::is_directory( Other ) )
	{
		return false;
	}
	else
	{
		return true;
	}
}

void FileHandler::UpdateDirectories( FS::path Path )
{
	std::vector< FS::path > Result;
	Result.push_back( Path );
	while( Path.generic_string() != "C:" )
	{
		PathGoUp( Path );
		Result.push_back( Path );
	}

    m_Directories = Result;
}

void FileHandler::UpdateFiles( std::string Filter, FS::path Path )
{
	if ( Path.generic_string() == "C:" )
	{
		std::string Temp = Path.generic_string() + "/";
		Path = Temp;
	}

	std::vector< FS::path > Result;

	FS::directory_iterator ITR( Path );
	while( ITR != FS::end( ITR ) )
	{
		const FS::directory_entry& ENTRY = *ITR;
		if ( CheckFilter( Filter, ENTRY.path() ) )
		{
			Result.push_back( ENTRY.path() );
		}
		ITR++;
	}

	sort( Result.begin(), Result.end(), Compare );

    m_Files = Result;
}

FileHandler::Data FileHandler::GetDirectories()
{
    return m_Directories;
}

FileHandler::Data FileHandler::GetFiles()
{
    return m_Files;
}

std::string FileHandler::GetFileName( FS::path Path )
{
	const char* charPath = nullptr;
	std::string stringPath = Path.generic_string();
	charPath = strrchr( stringPath.c_str(), '/' );

	if ( charPath != nullptr )
	{
		stringPath = std::string( charPath + 1);
	}

	return stringPath;
}

std::string FileHandler::GetFileExtension( FS::path Path )
{
	std::string FileName = GetFileName( Path );
	const char* FileExtension = nullptr;
	FileExtension = strrchr( FileName.c_str(), '.' );

	if ( FileExtension != nullptr )
	{
		return std::string( FileExtension + 1 );
	}
	else
	{
		return "";
	}

}

FileHandler& FileHandler::GetHandle() { return m_FileHandler; }

FileHandler FileHandler::m_FileHandler;
