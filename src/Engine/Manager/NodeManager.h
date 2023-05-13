#ifndef __NODEMANAGER_H__
#define __NODEMANAGER_H__

#include "Node.h"

class NodeManager
{
	template< typename T >
	using Vector = std::vector<T>;

	using Data = std::map< const type_info*, std::any >;

	private :
		NodeManager();
		~NodeManager();

	public :
		static NodeManager& GetHandle();
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
		Vector<T> GetVector()
		{
			bool Check = HasVector<T>();
			if ( !Check ) CreateVector<T>();

			return std::any_cast< Vector<T> >( m_Data[ &typeid( T ) ] );
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

			Log::Info(" Create --%s-- Node Vector ", Typename.c_str() );

			m_Data[ &typeid( T ) ] = Vector<T>();

			ClearVector<T>();
		}

		template< typename T >
		T* Get( MyUUID ID )
		{
			bool Check = HasVector<T>();
			if ( Check )
			{
				Vector<T>& TempVector = m_Data[ &typeid( T ) ];
				auto itr = std::find_if( TempVector.begin(), TempVector.end(), 
				[&]( const T& Node ) { return Node.m_ID == ID; } );

				if ( itr != TempVector.end() ) { return &(*itr);} 
				else { return nullptr; }
			}
			else{ return nullptr; }
		}

		template< typename T >
		void Create( Entity& Object )
		{
			bool Check = HasVector<T>();
			if ( !Check ) CreateVector<T>();

			GetVector<T>.emplace_back( T( Object ) );
		}

	private :
		static NodeManager m_NodeManager;
		Data m_Data;
};

#endif // __NODEMANAGER_H__