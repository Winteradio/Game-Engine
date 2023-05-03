#ifndef __SYSTEMMANAGER_H__
#define __SYSTEMMANAGER_H__

#include "Log.h"

class SystemManager
{
    using Data = std::map< const type_info*, std::any >;

	private :
        SystemManager();
        ~SystemManager();

    public :
        static SystemManager& GetHandle();

        template< typename T >
        void Clear()
        {

        }

        template< typename T >
        void RegisterSystem( T& System )
        {
            bool Result = HasSystem< T >();
            if ( !Result )
            {
                m_Data[ &typeid( T ) ] = std::make_any( System );
            }
        }

        template< typename T >
        bool HasSystem()
        {
            auto itr = m_Data.find( &typeid ( T ) );
            if ( itr != m_Data.end() ) 
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        template< typename T >
        void DeleteSystem()
        {
            bool Result = HasSystem< T >();
            if ( Result )
            {
                m_Data[ &typeid( T ) ].erase();
            }

        }

        void Init();
        void Destroy();

        Data& GetData();

    private :
        static SystemManager m_SystemManager;
        Data m_Data;
};

#endif // __SYSTEMMANAGER_H__