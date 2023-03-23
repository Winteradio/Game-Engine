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
		~MyUUID();

	public :
		void Init();
		void Destroy();
		std::string& GetID();

	private :
		std::string m_ID;
};

class MyUUIDCompare
{
	public :
		bool operator() ( MyUUID Id1, MyUUID Id2 ) const
		{
			if ( Id1.GetID() < Id2.GetID() )
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