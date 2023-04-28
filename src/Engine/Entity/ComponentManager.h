#ifndef __COMPONENTMANAGER_H__
#define __COMPONENTMANAGER_H__

#include "Component.h"

class ComponentManager
{
	template< typename T >
	using Vector = std::vector< T >;

	using Data = std::map< const type_info*, std::any >;

	public :
		ComponentManager();
		~ComponentManager();

	public :
		static ComponentManager& GetHandle();

		template< typename T >
		void CreateMap()
		{
			std::string Typename( typeid( T ).name() );
			Typename.erase( Typename.begin(), Typename.begin() + 6 );

			Log::Info(" Create Component Type's map -- %s -- ", Typename.c_str() );

			m_Data[ &typeid( T ) ] = std::vector< std::any >();

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

		void Init();
		void Destroy();

		Data& GetData();

	private :
		static ComponentManager m_ComponentManager;
		Data m_Data;
};

#endif // __COMPONENTMANAGER_H__