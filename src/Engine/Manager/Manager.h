#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "ManagerCollection.h"

#define MANAGER( Name ) m_##Name##Manager

class Manager
{
	public :
		Manager();
		~Manager();

	public :
		void Init();
		void Update();
		void Render();

		template < class T >
		static T*& Get()
		{
			if ( m_Manager.m_Data.find( &typeid( T ) ) == m_Manager.m_Data.end() )
			{
				CreateManager<T>();
			}

			return (T*)m_Manager.m_Data[ &typeid( T ) ];
		}

		template< class T >
		static void CreateManager()
		{
			T* NewManager = new T;
			m_Manager.m_Data[ &typeid( T ) ] = (void*)NewManager;
		}

	private :
		static Manager m_Manager;
		std::map< const std::type_info*, void* > m_Data;
};

#endif // __MANAGER_H__