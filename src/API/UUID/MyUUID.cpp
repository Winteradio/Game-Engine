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
}

bool MyUUID::operator == ( const MyUUID& Other ) const
{
	return m_ID == Other.m_ID;
}

MyUUID& MyUUID::operator = ( const std::string& ID )
{
	m_ID = ID;
	return *this;
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

bool MyUUID::Empty()
{
	if ( m_ID.empty() ) return true;
	else return false;
}

std::string& MyUUID::GetString() { return m_ID; }