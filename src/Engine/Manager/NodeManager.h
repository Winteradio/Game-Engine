#ifndef __NODEMANAGER_H__
#define __NODEMANAGER_H__

#include "Node.h"

class NodeManager
{
	template< typename T >
	using Vector = std::vector< T >;

	using Data = std::map< const type_info*, std::any >;

	private :
		NodeManager();
		~NodeManager();

	public :
		static NodeManager& GetHandle();

		template< typename T >
		void ClearVector()
		{
			m_Data[ &typeid( T ) ].clear();
		}

		template< typename T >
		void CreateVector()
		{
			std::string Typename( typeid( T ).name() );
			Typename.erase( Typename.begin(), Typename.begin() + 6 );

			Log::Info(" Create --%s-- Node Vector ", Typename.c_str() );

			m_Data[ &typeid( T ) ] = Vector< T >();
		}

		template< typename T >
		Vector<T>* GetVector()
		{
			bool Result = HasVector< T >();
			if ( Result )
			{
				return &std::any_cast< Vector< T > >( m_Data[ &typeid( T ) ] );
			}
			else
			{
				return nullptr;
			}
		}

		template< typename T >
		bool HasVector()
		{
			auto itr = m_Data.find( &typeid( T ) );
			if ( itr != m_Data.end() )
			{
				return true;
			}
			else
			{
				Log::Warn(" There is no node type of %s ", typeid( T ).name() );
				return false;
			}
		}

		template< typename T >
		T* GetNode( MyUUID ID )
		{
			bool Result = HasVector< T >();
			if ( Result )
			{
				Vector< T >& TempVector = m_Data[ &typeid( T ) ];
				auto itr = std::find_if( TempVector.begin(), TempVector.end(), 
				[&]( const T& Node ) { return Node.m_ID == ID; } );

				if ( itr != TempVector.end() )
				{
					return &(*itr);
				} 
				else
				{
					return nullptr;
				}
			}
			else
			{
				return nullptr;
			}
		}

		template< typename T >
		void CreateNode( Entity& Object )
		{
			bool Result = HasVector< T >();
			if ( !Result )
			{
				CreateVector< T > ();
			}

			m_Data[ &typeid( T ) ].emplace_back( T( Object ) );
		}

		void Init();
		void Destroy();

		Data& GetData();

	private :
		static NodeManager m_NodeManager;
		Data m_Data;
};

#endif // __NODEMANAGER_H__