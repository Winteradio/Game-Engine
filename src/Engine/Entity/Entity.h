#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Log.h"
#include "Component.h"
#include "MyUUID.h"

class Entity
{
	using Data = std::map< const std::type_info*, std::any >;

	public :
		Entity();
		virtual ~Entity();

	public :
		template< typename T >
		void AddComponent( T* Comp )
		{
			bool Result = HasComponent<T>();
			if ( !Result )
			{
				m_Data[ &typeid( T ) ] = Comp;
			}
		}

		template< typename T >
		void RemoveComponent()
		{
			bool Result = HasComponent<T>();
			if ( Result )
			{
				m_Data.erase( &typeid( T ) );
			}
		}

		template< typename T >
		T* GetComponent()
		{
			bool Result = HasComponent<T>();
			if ( Result )
			{
				return std::any_cast< T* >( m_Data[ &typeid( T ) ] );
			}
			else
			{
				return nullptr;
			}
		}

		template< typename T >
		bool HasComponent()
		{
			auto itr = m_Data.find( &typeid( T ) );
			if ( itr != m_Data.end() ) { return true; }
			else { return false; }
		}

		MyUUID& GetID();
		std::string& GetIDString();
		std::string& GetName();
		Data& GetData();
		void Destroy();

	private :
		Data m_Data;
};

#endif // __ENTITY_H__