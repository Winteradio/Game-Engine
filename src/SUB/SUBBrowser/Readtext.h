#ifndef __READTEXT_H__
#define __READTEXT_H__

#define MAXLENGTH 256

#define ALL_ORIGIN_FILE 0
#define SKIP_ORIGIN_FILE 1
#define OVERWRITE_ORIGIN_FILE 2
#define UPDATE_ORIGIN_FILE 3

#include "SUBLog.h"
#include <string>
#include <optional>
#include <filesystem>
#include <windows.h>

#include <fstream>
#include <iostream>

typedef enum PATHTYPE
{
	NULLPATH,
	FILEPATH,
	DIRECTORYPATH
}PATHTYPE;

namespace FS = std::filesystem;

namespace READTEXT
{
	extern std::ifstream MyFile;

	extern char FileName[ MAXLENGTH ];
	extern bool Play;

	extern FS::path MainPath;
	extern FS::path TempPath;

	void FileLoad( FS::path Path );
	void FileRead( FS::path Path );

	void FileCopy( FS::path OriginPath, FS::path NewPath, int OriginFileHandling );
	void FileMove( FS::path OriginPath, FS::path NewPath );
	void FileDelete( FS::path Path );

	bool CheckExisted( FS::path Path );
	PATHTYPE CheckPathType( FS::path Path );
	void CheckFileType( FS::path Path );

	void TypeWindow( FS::path Path );
	void TypeObject( FS::path Path );
	void TypeStory( FS::path Path );
	void TypeBrowser( FS::path Path );

	void PrintCurrentPath();
	void PrintRelativePath( FS::path Path );
	void PrintAbsolutePath( FS::path Path );
	void PrintCanonicalAbsoluatePath( FS::path Path );

	void PrintFilesinDirectory( FS::path Path );
	void PrintAllFilesinDirectory( FS::path Path );

	void CreateDirectory( FS::path Path );
	void CreateAllDirectory( FS::path Path );

	void PathGo2Up( FS::path& Path );
	void PathGo2Down( FS::path& Path, char* DownPath, bool Read );

	char* GetFileName( const char* Path );

	void Frame();
}

#endif // __READTEXT_H__