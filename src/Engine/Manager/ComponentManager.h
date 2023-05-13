#ifndef __COMPONENTMANAGER_H__
#define __COMPONENTMANAGER_H__

#include "Component.h"

class ComponentManager
{
	template< typename T >
	using Vector = std::vector<T>;

	using Data = std::map< const type_info*, std::any >;

	private :
		ComponentManager();
		~ComponentManager();

	public :
		static ComponentManager& GetHandle();
		void Init();
		void Destroy();
		Data& GetData();

	public :
		template< typename T >
		bool HasVector()
		{
			if ( m_Data.find( &typeid( T ) ) == m_Data.end() ) return false;
			else return true;
		}

		template< typename T >
		Vector<T>& GetVector()
		{
			bool Check = HasVector<T>();
			if ( !Check ) CreateVector<T>();

			return std::any_cast< Vector<T>& >( m_Data[ &typeid( T ) ] );
		}

		template< typename T >
		void ClearVector()
		{
			bool Check = HasVector<T>();
			if ( Check ) GetVector<T>().clear();
		}

		template< typename T >
		void CreateVector()
		{
			std::string Typename( typeid( T ).name() );
			Typename.erase( Typename.begin(), Typename.begin() + 6 );

			Log::Info(" Create -- %s -- Component Vector ", Typename.c_str() );

			m_Data[ &typeid( T ) ] = Vector<T>();

			ClearVector<T>();
		}

		template< typename T, typename... Args >
		T* Create( Args&&... args )
		{
			bool Check = HasVector<T>();
			if ( !Check ) CreateVector<T>();
			GetVector<T>().emplace_back( T( std::forward< Args >( args )... ) );

			return &GetVector<T>().back();
		}

	private :
		static ComponentManager m_ComponentManager;
		Data m_Data;
};

#endif // __COMPONENTMANAGER_H__