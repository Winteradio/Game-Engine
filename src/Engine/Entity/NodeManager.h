#ifndef __NODEMANAGER_H__
#define __NODEMANAGER_H__

#include "Nodes.h"

class NodeManager
{
	using NodeData = std::map< const type_info*, std::vector< void* > >;

	public :
		NodeManager();
		~NodeManager();

	public :
		template< typename T >
		void CreateMap()
		{
			std::string Typename( typeid( T ).name() );
			Typename.erase( Typename.begin(), Typename.begin() + 6 );

			Log::Info(" Create Node Type's map -- %s -- ", Typename.c_str() );

			m_Data[ &typeid( T ) ] = std::vector< void * >();

			ClearMap<T>();
		}

		template< typename T >
		void ClearMap()
		{
			m_Data[ &typeid( T ) ].clear();
		}

		template< typename T >
		void Create()
		{
			m_Data[ &typeid( T ) ].emplace_back( ( void* )( new T() ) );
		}

		NodeData& GetNodes();

	private :
		NodeData m_Data;
};

#endif // __NODEMANAGER_H__