#ifndef __DIFILEBROWSER_H__
#define __DIFILEBROWSER_H__

#include "DXImGUICommon.h"
#include "DXWindow.h"
#include <string>
#include <vector>
#include <optional>
#include <filesystem>

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
		bool Init();
		bool Frame();
		void Render( bool& Rendering );
		void Release();

	private :
		void InitPointer();

		void RenderFolderButton( FS::path Path );
		void RenderFileFilter();
		void RenderFilesSelectable( FS::path Path, std::vector<FILEDATA> FileData );
		void RenderEndButton( bool& Rendering );

		std::vector<FILEDATA> CheckFilesbyFilter( FS::path Path );
		void CheckResult();
		bool CheckExisted( FS::path Path );
		PATHTYPE CheckPathType( FS::path Path );

		bool SaveFile();
		bool OpenFile();

		void PathGo2Up( FS::path& Path );
		void PathGo2Down( FS::path TempPath, FS::path& MainPath );

		std::vector<std::string> Split(std::string Input, char Diameter );

		static bool Compare( const FILEDATA& Num1, const FILEDATA& Num2 );

	public :
		void GetFileName( const char* Path );
		char* GetFilePath();

	private :
		char m_TempFile[MAXLENGTH];
		char m_FileFilter[MAXLENGTH];
		char m_FileName[MAXLENGTH];
		char m_ResultPath[MAXLENGTH];

		std::vector<std::string> m_FolderData;

		FS::path m_OriginPath;
		FS::path m_MainPath;
		FS::path m_TempPath;

		int m_ColumnElement;

		DXRECTWINDOW* m_BrowserRect;
};

#endif // __DIFILEBROWSER_H__