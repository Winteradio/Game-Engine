#include "ProjectManager.h"

ProjectManager::ProjectManager() {}

ProjectManager::~ProjectManager() {}

void ProjectManager::LoadFile( FS::path Path )
{
	Log::Info(" Load File %s", Path.generic_string().c_str() );

	YAML::Node FileData = YAML::LoadFile(Path.generic_string());

	for ( auto Data : FileData )
	{
		if ( LoadString( Data["Type"] ) == typeid(Scene).name() ) LoadScene( Data );
		else if ( LoadString( Data["Type"] ) == typeid(Component).name() ) LoadComponent( Data );
		else if ( LoadString( Data["Type"] ) == typeid(Camera).name() ) LoadCamera( Data );
		else if ( LoadString( Data["Type"] ) == typeid(Light).name() ) LoadLight( Data );
		else if ( LoadString( Data["Type"] ) == typeid(Mesh).name() ) LoadMesh( Data );
		else if ( LoadString( Data["Type"] ) == typeid(Script).name() ) LoadScript( Data );
		else if ( LoadString( Data["Type"] ) == typeid(Shader).name() ) LoadShader( Data );
		else if ( LoadString( Data["Type"] ) == typeid(Texture).name() ) LoadTexture( Data );
	}
}

// LoadFile
void ProjectManager::LoadScene( YAML::Node& FileData )
{
	LoadCommon<Scene>( FileData );

	MyUUID ID( LoadString( FileData["UUID"] ) );

	LoadTranslation<Scene>( FileData, ID );
	LoadRotation<Scene>( FileData, ID );
	LoadScale<Scene>( FileData, ID );

	for ( auto Data : FileData["Part"] )
	{
		if ( LoadString( Data["Type"] ) == typeid(Component).name() )
		{
			ResourceManager::GetResource<Scene>(ID)->InputResource<Component>( LoadString( Data["UUID"] ) );
		}
		else if ( LoadString( Data["Type"] ) == typeid(Camera).name() )
		{
			ResourceManager::GetResource<Scene>(ID)->InputResource<Camera>( LoadString( Data["UUID"] ) );
		}
		else if ( LoadString( Data["Type"] ) == typeid(Light).name() )
		{
			ResourceManager::GetResource<Scene>(ID)->InputResource<Light>( LoadString( Data["UUID"] ) );
		}
	}
}

void ProjectManager::LoadComponent( YAML::Node& FileData )
{
	LoadCommon<Component>( FileData );

	MyUUID ID( LoadString( FileData["UUID"] ) );

	LoadTranslation<Component>( FileData, ID );
	LoadRotation<Component>( FileData, ID );
	LoadScale<Component>( FileData, ID );

	for ( auto Data : FileData["Part"] )
	{
		if ( LoadString( Data["Type"] ) == typeid(Mesh).name() )
		{
			ResourceManager::GetResource<Component>(ID)->InputResource<Mesh>( LoadString( Data["UUID"] ) );
		}
		else if ( LoadString( Data["Type"] ) == typeid(Script).name() )
		{
			ResourceManager::GetResource<Component>(ID)->InputResource<Script>( LoadString( Data["UUID"] ) );
		}
		else if ( LoadString( Data["Type"] ) == typeid(Shader).name() )
		{
			ResourceManager::GetResource<Component>(ID)->InputResource<Shader>( LoadString( Data["UUID"] ) );
		}
		else if ( LoadString( Data["Type"] ) == typeid(Texture).name() )
		{
			ResourceManager::GetResource<Component>(ID)->InputResource<Texture>( LoadString( Data["UUID"] ) );
		}
	}
}

void ProjectManager::LoadCamera( YAML::Node& FileData )
{
	LoadCommon<Camera>( FileData );

	MyUUID ID( LoadString( FileData["UUID"] ) );

	LoadTranslation<Camera>( FileData, ID );
	LoadRotation<Camera>( FileData, ID );
	LoadScale<Camera>( FileData, ID );
}

void ProjectManager::LoadLight( YAML::Node& FileData )
{
	LoadCommon<Light>( FileData );

	MyUUID ID( LoadString( FileData["UUID"] ) );

	LoadTranslation<Light>( FileData, ID );
	LoadRotation<Light>( FileData, ID );
	LoadScale<Light>( FileData, ID );
}

void ProjectManager::LoadMesh( YAML::Node& FileData )
{
	LoadCommon<Mesh>( FileData );

	MyUUID ID( LoadString( FileData["UUID"] ) );

	LoadFilePath<Mesh>( FileData, ID );
}

void ProjectManager::LoadScript( YAML::Node& FileData )
{
	LoadCommon<Script>( FileData );

	MyUUID ID( LoadString( FileData["UUID"] ) );

	LoadFilePath<Script>( FileData, ID );
}

void ProjectManager::LoadShader( YAML::Node& FileData )
{
	LoadCommon<Shader>( FileData );

	MyUUID ID( LoadString( FileData["UUID"] ) );

	LoadFilePath<Shader>( FileData, ID );
}

void ProjectManager::LoadTexture( YAML::Node& FileData )
{
	LoadCommon<Texture>( FileData );

	MyUUID ID( LoadString( FileData["UUID"] ) );

	LoadFilePath<Texture>( FileData, ID );
}

XMFLOAT2 ProjectManager::LoadXMFLOAT2( YAML::Node& FileData )
{
	return XMFLOAT2( FileData[0].as<float>(), FileData[1].as<float>() );
}

XMFLOAT3 ProjectManager::LoadXMFLOAT3( YAML::Node& FileData )
{
	return XMFLOAT3( FileData[0].as<float>(), FileData[1].as<float>(), FileData[2].as<float>() );
}

XMFLOAT4 ProjectManager::LoadXMFLOAT4( YAML::Node& FileData )
{
	return XMFLOAT4( FileData[0].as<float>(), FileData[1].as<float>(), FileData[2].as<float>(), FileData[3].as<float>() );
}

std::string ProjectManager::LoadString( YAML::Node& FileData )
{
	return FileData.as<std::string>();
}

// SaveFile
void ProjectManager::SaveFile( FS::path Path )
{
	Log::Info(" Save File %s", Path.generic_string().c_str() );
	YAML::Emitter Data;
	Data << YAML::BeginSeq;
	for ( auto ITRResourceMap : ResourceManager::Get() )
	{
		if ( !ITRResourceMap.second.empty() )
		{
			for ( auto ITRResource : ITRResourceMap.second )
			{
				IResource* Resource = ITRResource.second;

				if ( Resource->GetType()->name() == typeid(Scene).name() ) SaveScene( Data, Resource );
				else if ( Resource->GetType()->name() == typeid(Component).name() ) SaveComponent( Data, Resource );
				else if ( Resource->GetType()->name() == typeid(Camera).name() ) SaveCamera( Data, Resource );
				else if ( Resource->GetType()->name() == typeid(Light).name() ) SaveLight( Data, Resource );
				else if ( Resource->GetType()->name() == typeid(Mesh).name() ) SaveMesh( Data, Resource );
				else if ( Resource->GetType()->name() == typeid(Script).name() ) SaveScript( Data, Resource );
				else if ( Resource->GetType()->name() == typeid(Shader).name() ) SaveShader( Data, Resource );
				else if ( Resource->GetType()->name() == typeid(Texture).name() ) SaveTexture( Data, Resource );
				Data << YAML::Newline;
			}
		}
	}
	Data << YAML::EndSeq;

	std::ofstream fout( Path, std::ios_base::out );
	fout << Data.c_str();
}

void ProjectManager::SaveCommon( YAML::Emitter& Data, IResource* Resource )
{
	Data << YAML::BeginMap;
	Data << YAML::Key << "Type" << YAML::Value << Resource->GetType()->name();
	Data << YAML::Key << "Name" << YAML::Value << Resource->GetName();
	Data << YAML::Key << "UUID" << YAML::Value << Resource->GetUUID();
}

void ProjectManager::SaveScene( YAML::Emitter& Data, IResource* Resource )
{
	SaveCommon( Data, Resource );
	Scene* TempResource = (Scene*)Resource;

	SaveXMFLOAT3( Data, TempResource->GetTranslation(), "Translation" );
	SaveXMFLOAT3( Data, TempResource->GetRotation(), "Rotation" );
	SaveXMFLOAT3( Data, TempResource->GetScale(), "Scale" );

	Data << YAML::Key << "Part";
	Data << YAML::Value;
	Data << YAML::BeginSeq;
	for( auto ITR : TempResource->GetData() )
	{
		if ( !ITR.second.empty() )
		{
			Data << YAML::BeginMap;
			Data << YAML::Key << "Type" << YAML::Value << ITR.first->name();
			for ( auto ID : ITR.second )
			{
				Data << YAML::Key << "UUID" << YAML::Value << ID.GetID();
			}
			Data << YAML::EndMap;
		}
	}
	Data << YAML::EndSeq;

	Data << YAML::EndMap;
}

void ProjectManager::SaveComponent( YAML::Emitter& Data, IResource* Resource )
{
	SaveCommon( Data, Resource );
	Component* TempResource = (Component*)Resource;

	SaveXMFLOAT3( Data, TempResource->GetTranslation(), "Translation" );
	SaveXMFLOAT3( Data, TempResource->GetRotation(), "Rotation" );
	SaveXMFLOAT3( Data, TempResource->GetScale(), "Scale" );

	Data << YAML::Key << "Part";
	Data << YAML::Value;
	Data << YAML::BeginSeq;
	for( auto ITR : TempResource->GetData() )
	{
		if ( !ITR.second.empty() )
		{
			Data << YAML::BeginMap;
			Data << YAML::Key << "Type" << YAML::Value << ITR.first->name();
			for ( auto ID : ITR.second )
			{
				Data << YAML::Key << "UUID" << YAML::Value << ID.GetID();
			}
			Data << YAML::EndMap;
		}
	}
	Data << YAML::EndSeq;

	Data << YAML::EndMap;
}

void ProjectManager::SaveCamera( YAML::Emitter& Data, IResource* Resource )
{
	SaveCommon( Data, Resource );
	Camera* TempResource = (Camera*)Resource;

	SaveXMFLOAT3( Data, TempResource->GetTranslation(), "Translation" );
	SaveXMFLOAT3( Data, TempResource->GetRotation(), "Rotation" );
	SaveXMFLOAT3( Data, TempResource->GetScale(), "Scale" );

	Data << YAML::EndMap;
}

void ProjectManager::SaveLight( YAML::Emitter& Data, IResource* Resource )
{
	SaveCommon( Data, Resource );
	Light* TempResource = (Light*)Resource;

	SaveXMFLOAT3( Data, TempResource->GetTranslation(), "Translation" );
	SaveXMFLOAT3( Data, TempResource->GetRotation(), "Rotation" );
	SaveXMFLOAT3( Data, TempResource->GetScale(), "Scale" );

	Data << YAML::EndMap;
}

void ProjectManager::SaveMesh( YAML::Emitter& Data, IResource* Resource )
{
	SaveCommon( Data, Resource );
	Mesh* TempResource = (Mesh*)Resource;

	SaveFilePath( Data, TempResource->GetFilePath() );

	Data << YAML::EndMap;
}

void ProjectManager::SaveScript( YAML::Emitter& Data, IResource* Resource )
{
	SaveCommon( Data, Resource );
	Script* TempResource = (Script*)Resource;

	SaveFilePath( Data, TempResource->GetFilePath() );

	Data << YAML::EndMap;
}

void ProjectManager::SaveShader( YAML::Emitter& Data, IResource* Resource )
{
	SaveCommon( Data, Resource );
	Shader* TempResource = (Shader*)Resource;

	SaveFilePath( Data, TempResource->GetFilePath() );

	Data << YAML::EndMap;
}

void ProjectManager::SaveTexture( YAML::Emitter& Data, IResource* Resource )
{
	SaveCommon( Data, Resource );
	Texture* TempResource = (Texture*)Resource;

	SaveFilePath( Data, TempResource->GetFilePath() );

	Data << YAML::EndMap;
}

void ProjectManager::SaveXMFLOAT2( YAML::Emitter& Data, XMFLOAT2 Info, std::string DataName )
{
	Data << YAML::Key << DataName;
	Data << YAML::Flow;
	Data << YAML::BeginSeq;
	Data << YAML::Value << Info.x << Info.y;
	Data << YAML::EndSeq;
}

void ProjectManager::SaveXMFLOAT3( YAML::Emitter& Data, XMFLOAT3 Info, std::string DataName )
{
	Data << YAML::Key << DataName;
	Data << YAML::Flow;
	Data << YAML::BeginSeq;
	Data << YAML::Value << Info.x << Info.y << Info.z;
	Data << YAML::EndSeq;
}

void ProjectManager::SaveXMFLOAT4( YAML::Emitter& Data, XMFLOAT4 Info, std::string DataName )
{
	Data << YAML::Key << DataName;
	Data << YAML::Flow;
	Data << YAML::BeginSeq;
	Data << YAML::Value << Info.x << Info.y << Info.z << Info.w;
	Data << YAML::EndSeq;
}

void ProjectManager::SaveFilePath( YAML::Emitter& Data, FS::path Path )
{
	Data << YAML::Key << "FilePath";
	Data << YAML::Value << Path.generic_string();
}
ProjectManager ProjectManager::m_ProjectManager;
