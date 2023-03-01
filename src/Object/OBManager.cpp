#include "OBManager.h"

OBMANAGER::OBMANAGER()
{
	InitPointer();
}

OBMANAGER::OBMANAGER( const OBMANAGER* Other )
{
	*this = *Other;
}

OBMANAGER::~OBMANAGER() {}

bool OBMANAGER::Init()
{

}

bool OBMANAGER::CreateModel()
{

}

bool OBMANAGER::CreateShader()
{

}

void OBMANAGER::SetShaderToModel( OBMODEL* model, OBSHADER* shader )
{

}

OBMODEL* OBMANAGER::GetModels() { return m_Models; }
OBSHADER* OBMANAGER::GetShaders() { return m_Shaders; }

OBMODEL OBMANAGER::GetModel( int index ) { return m_Models[ index ]; }
OBSHADER OBMANAGER::GetShader( int index ) { return m_Shaders[ index ]; }