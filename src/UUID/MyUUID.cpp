#include "MyUUID.h"

MyUUID::MyUUID()
{
}

MyUUID::MyUUID( std::string ID )
{
	m_ID = ID;
}

MyUUID::~MyUUID()
{
	Destroy();
}

MyUUID::MyUUID( const MyUUID& Other )
{
	m_ID = Other.m_ID;
}

void MyUUID::Init()
{
	UUID Id;
	UuidCreate( &Id );

	char* UuidChar;
	UuidToStringA( &Id, (RPC_CSTR*)&UuidChar );

	m_ID = std::string( UuidChar );

	Log::Info(" Create UUID - %s", m_ID.c_str() );
}

void MyUUID::Destroy()
{
	m_ID.clear();
}

std::string& MyUUID::GetID() { return m_ID; }