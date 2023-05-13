#ifndef __FILEBROWSERGUILAYER_H__
#define __FILEBROWSERGUILAYER_H__

#include "FileDialog.h"
#include "IBrowserGuiLayer.h"

class FileBrowserGuiLayer : public IBrowserGuiLayer
{
	public :
		FileBrowserGuiLayer();
		virtual ~FileBrowserGuiLayer();

	public :
		static FileBrowserGuiLayer& Get();
		virtual void Init( int PosX, int PosY, int Width, int Height );
		virtual void Destroy();
		virtual void Update();
		virtual void Render();
		virtual void SetPath( FS::path Path );
		void RenderDirectory();
		void RenderFilter();
		void RenderFolderInFiles( std::string Extension );
		void RenderSelectedFiles();
		void Begin( IBrowserGuiLayer* Layer, std::string Extension, bool Load );
		void End();

	private :
		static FileBrowserGuiLayer m_FileBrowserGuiLayer;

		std::vector< FS::path > m_Directories;
		std::vector< FS::path > m_Files;

		std::string m_Filter;
		std::string m_Extension;

		std::set< FS::path > m_SelectedFiles;

		std::filesystem::path m_CurrentPath;
		std::filesystem::path m_MainPath;

		char m_Temp[ MAX_LENGTH ];

		int m_Columns;
		int m_ColumnElement;

		bool m_Mode;

		IBrowserGuiLayer* m_RequringPathLayer;
};

#endif // __FILEBROWSERGUILAYER_H__