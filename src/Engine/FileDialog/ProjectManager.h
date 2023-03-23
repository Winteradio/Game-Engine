#ifndef __PROJECTMANAGER_H__
#define __PROJECTMANAGER_H__

#include "yaml-cpp/yaml.h"
#include "yaml-cpp/Parser.h"

#include "FileDialog.h"
#include "ResourceManager.h"

class ProjectManager
{
	private :
		ProjectManager();
		~ProjectManager();

	public :
		static ProjectManager& Get()
		{
			return m_ProjectManager;
		}
		void LoadFile( FS::path Path );
		void LoadScene( YAML::Node& FileData );
		void LoadComponent( YAML::Node& FileData );
		void LoadCamera( YAML::Node& FileData );
		void LoadLight( YAML::Node& FileData );
		void LoadMesh( YAML::Node& FileData );
		void LoadScript( YAML::Node& FileData );
		void LoadShader( YAML::Node& FileData );
		void LoadTexture( YAML::Node& FileData );

		float LoadFloat( YAML::Node& FileData ) {};
		XMFLOAT2 LoadXMFLOAT2( YAML::Node& FileData ) {};
		XMFLOAT3 LoadXMFLOAT3( YAML::Node& FileData );
		XMFLOAT4 LoadXMFLOAT4( YAML::Node& FileData ) {};
		std::string LoadString( YAML::Node& FileData );

		template< class T >
		void LoadCommon( YAML::Node& FileData )
		{
			ResourceManager::CreateResource< T >( LoadString( FileData["UUID"] ), LoadString( FileData["Name"] ) );
		}
		template< class T >
		void LoadTranslation( YAML::Node& FileData, MyUUID ID )
		{
			ResourceManager::GetResource<T>( ID )->SetTranslation( LoadXMFLOAT3( FileData["Translation"] ) );
		}
		template< class T >
		void LoadRotation( YAML::Node& FileData, MyUUID ID )
		{
			ResourceManager::GetResource<T>( ID )->SetRotation( LoadXMFLOAT3( FileData["Rotation"] ) );
		}
		template< class T >
		void LoadScale( YAML::Node& FileData, MyUUID ID )
		{
			ResourceManager::GetResource<T>( ID )->SetScale( LoadXMFLOAT3( FileData["Scale"] ) );
		}
		template< class T >
		void LoadFilePath( YAML::Node& FileData, MyUUID ID )
		{
			ResourceManager::GetResource<T>( ID )->SetFilePath( LoadString( FileData["FilePath"] ) );
		}


		void SaveFile( FS::path Path );
		void SaveCommon( YAML::Emitter& Data, IResource* Resource );
		void SaveScene( YAML::Emitter& Data, IResource* Resource );
		void SaveComponent( YAML::Emitter& Data, IResource* Resource );
		void SaveCamera( YAML::Emitter& Data, IResource* Resource );
		void SaveLight( YAML::Emitter& Data, IResource* Resource );
		void SaveMesh( YAML::Emitter& Data, IResource* Resource );
		void SaveScript( YAML::Emitter& Data, IResource* Resource );
		void SaveShader( YAML::Emitter& Data, IResource* Resource );
		void SaveTexture( YAML::Emitter& Data, IResource* Resource );

		void SaveFloat( YAML::Emitter& Data, float Info, std::string DataName ) {};
		void SaveXMFLOAT2( YAML::Emitter& Data, XMFLOAT2 Info, std::string DataName ) {};
		void SaveXMFLOAT3( YAML::Emitter& Data, XMFLOAT3 Info, std::string DataName );
		void SaveXMFLOAT4( YAML::Emitter& Data, XMFLOAT4 Info, std::string DataName ) {};
		void SaveFilePath( YAML::Emitter& Data, FS::path Path );

	private :
		static ProjectManager m_ProjectManager;
};

#endif // __PROJECTMANAGER_H__