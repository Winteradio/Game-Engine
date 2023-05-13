#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Log.h"
#include "Component.h"
#include "MyUUID.h"

class Entity
{
	using Data = std::map< const std::type_info*, int >;

	public :
		Entity();
		Entity( std::string Name );
		Entity( MyUUID ID );
		Entity( MyUUID ID, std::string Name );
		virtual ~Entity();

	public :
		template< typename T >
		void AddComponent( int Index )
		{
			bool Result = HasComponent<T>();
			if ( !Result ) m_Data[ &typeid( T ) ] = Index;
		}

		template< typename T >
		void RemoveComponent()
		{
			bool Result = HasComponent<T>();
			if ( Result ) m_Data.erase( &typeid( T ) );
		}

		template< typename T >
		int GetComponent()
		{
			bool Result = HasComponent<T>();
			if ( Result ) { return m_Data[ &typeid( T ) ]; }
			else { return -1; }
		}

		template< typename T >
		bool HasComponent()
		{
			auto itr = m_Data.find( &typeid( T ) );
			if ( itr != m_Data.end() ) { return true; }
			else { return false; }
		}

	public :
		void SetID( MyUUID ID );
		void SetName( std::string Name );

		Data& GetData();
		MyUUID& GetID();
		std::string& GetName();
		void Destroy();

	private :
		Data m_Data;
		MyUUID m_ID;
		std::string m_Name;
};

#endif // __ENTITY_H__