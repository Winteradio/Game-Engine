#include "Scene.h"

SCENE::SCENE()
{

}

SCENE::SCENE( const SCENE& Other )
{

}

SCENE::~SCENE()
{

}

void SCENE::Release()
{

}

void SCENE::Frame()
{
	int Size = SceneData.size();

	for ( int I = 0; I < Size; I++ )
	{
		SceneData[ I ]->Frame();
	}
}

void SCENE::Render()
{
	int Size = SceneData.size();

	for ( int I = 0; I < Size; I++ )
	{
		SceneData[ I ]->Render();
	}
}

void SCENE::SetResource( const IRESOURCE* Resource )
{
	this->SceneData.push_back( Resource );
}

IRESOURCE* SCENE::GetResource()
{
}