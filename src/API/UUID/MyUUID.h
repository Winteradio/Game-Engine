#ifndef __MYUUID_H__
#define __MYUUID_H__

#include "Utils.h"
#include <Rpc.h>
#include "Log.h"

class MyUUID
{
	public :
		MyUUID();
		MyUUID( std::string ID );
		MyUUID( const MyUUID& Other );
		MyUUID& operator = ( const std::string& ID );
		~MyUUID();

	public :
		void Init();
		std::string& GetString();

	private :
		std::string m_ID;
};

class MyUUIDCompare
{
	public :
		bool operator() ( MyUUID Id1, MyUUID Id2 ) const
		{
			if ( Id1.GetString() < Id2.GetString() )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
};

#endif // __MYUUID_H__