#include "FileDialog.h"

FileDialog::FileDialog() {}

FileDialog::~FileDialog() {}

void FileDialog::PathGoUp( FS::path& Path )
{
	std::string stringPath = Path.generic_string();
	std::string ErasedPath = "/" + GetFileName( Path );
	stringPath.erase( stringPath.end() - ErasedPath.length(), stringPath.end() );

	Path = stringPath;
}

void FileDialog::PathGoDown( FS::path FolderPath, FS::path& MainPath )
{
	MainPath = FolderPath;
}

std::vector< FS::path > FileDialog::GetDirectories( FS::path Path )
{
	std::vector< FS::path > Result;
	Result.push_back( Path );
	while( Path.generic_string() != "C:" )
	{
		PathGoUp( Path );
		Result.push_back( Path );
	}

	return Result;
}

std::vector< FS::path > FileDialog::GetFiles( std::string Filter, FS::path Path )
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

	return Result;
}

std::string FileDialog::GetFileName( FS::path Path )
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

bool FileDialog::IsPathFile( FS::path Path )
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

bool FileDialog::Compare( FS::path OnePath, FS::path OtherPath )
{
	std::string stringOne = OnePath.generic_string();
	std::string stringOther = OtherPath.generic_string();
	if ( FS::is_regular_file( OnePath ) && FS::is_regular_file( OtherPath ) )
	{

		return stringOne < stringOther;
	}
	else if ( FS::is_directory( OnePath ) && FS::is_directory( OtherPath ) )
	{
		return stringOne < stringOther;
	}
	else if ( FS::is_regular_file( OnePath ) && FS::is_directory( OtherPath ) )
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool FileDialog::CheckFilter( std::string Filter, FS::path Path )
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

FileDialog FileDialog::m_FileDialog;