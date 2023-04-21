#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Log.h"
#include "Components.h"

class Entity
{
	using ComponentData = std::map< const type_info*, void* >;

	public :
		Entity();
		Entity( std::string Name );
		Entity( std::string Name, MyUUID ID );
		virtual ~Entity();

	public :
		template< typename T >
		void AddComponent( T*& Comp )
		{
			bool Result = HasComponent< T >();
			if ( !Result )
			{
				m_Data[ &typeid( T ) ] = static_cast< void* >( Comp );
			}
		}

		template< typename T >
		void RemoveComponent()
		{
			bool Result = HasComponent< T >();
			if ( Result )
			{
				m_Data.erase[ &typeid( T ) ];
			}
		}

		template< typename T >
		T& GetComponent()
		{
			bool Result = HasComponent< T >();
			if ( Result )
			{
				return static_cast<T>(*m_Data[ &typeid( T ) ]);
			}
		}

		template< typename T >
		bool HasComponent()
		{
			auto itr = m_Data.find( &typeid( T ) );
			if ( itr != m_Data.end() )
			{
				return true;
			}
			else
			{
				Log::Warn("There is no component type of %s ", typeid( T ).name() );
				return false;
			}
		}

		ComponentData& GetComponents();

		void SetID( MyUUID ID );
		void SetName( std::string Name );
		std::string& GetID();
		std::string& GetName();

	private :
		MyUUID m_ID;
		std::string m_Name;
		ComponentData m_Data;
};

#endif // __ENTITY_H__