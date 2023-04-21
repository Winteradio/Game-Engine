#ifndef __COMPONENTMANAGER_H__
#define __COMPONENTMANAGER_H__

#include "Components.h"

class ComponentManager
{
	using ComponentData = std::map< const type_info*, std::vector< void* > >;

	public :
		ComponentManager();
		~ComponentManager();

	public :
		template< typename T >
		void CreateMap()
		{
			std::string Typename( typeid( T ).name() );
			Typename.erase( Typename.begin(), Typename.begin() + 6 );

			Log::Info(" Create Component Type's map -- %s -- ", Typename.c_str() );

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

		ComponentData& GetComponents();

	private :
		ComponentData m_Data;
};

#endif // __COMPONENTMANAGER_H__