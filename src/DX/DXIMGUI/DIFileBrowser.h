#ifndef __DIFILEBROWSER_H__
#define __DIFILEBROWSER_H__

#include "DXImGUICommon.h"
#include "DXWindow.h"
#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include <windows.h>

#include <fstream>
#include <iostream>
#include <algorithm>

#define MAXLENGTH 256

namespace FS = std::filesystem;

enum class PATHTYPE
{
	FOLDER,
	FILE
};

struct FILEDATA
{
	PATHTYPE Type;
	FS::path Path;
};

class DIFILEBROWSER
{
	public :
		DIFILEBROWSER();
		~DIFILEBROWSER();

	public :
		void InitPointer();
		bool Frame();
		void Render( bool& Rendering );

		bool SaveFile();
		bool OpenFile();

		void PrintAllFilesInDirectory( FS::path Path );
		void PathGo2Up( FS::path& Path );
		void PathGo2Down( FS::path TempPath, FS::path& MainPath );

		bool CheckExisted( FS::path Path );
		PATHTYPE CheckPathType( FS::path Path );

		void GetFileName( const char* Path );

		std::vector<std::string> split(std::string input, char delimiter);

		static bool Compare( const FILEDATA& Num1, const FILEDATA& Num2 );



	private :
		char m_TempFile[MAXLENGTH];
		char m_FileFilter[MAXLENGTH];
		char m_FileName[MAXLENGTH];
		std::ifstream m_MyFile;

		FS::path m_MainPath;
		FS::path m_TempPath;

		int m_ColumnElement;

		DXRECTWINDOW* m_BrowserRect;

		char m_ResultPath[MAXLENGTH];
};

#endif // __DIFILEBROWSER_H__