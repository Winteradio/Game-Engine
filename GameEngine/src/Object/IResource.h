#ifndef __IRESOURCE_H__
#define __IRESOURCE_H__

#include "Rpc.h"
#include "string"
#include "DXResourceType.h"

class IRESOURCE
{
	public :
		UUID ID = NULL;
		std::string Name;
		RESOURCETYPE Type;

	public :
		IRESOURCE();
		IRESOURCE( const IRESOURCE& Other);
		virtual ~IRESOURCE();

	public :
		void CreateID()
		{
			if ( ID = NULL )
			{
				UuidCreate( &this->ID );
			}
		}

		bool Init( const std::string& Name, const RESOURCETYPE& Type )
		{
			this->Name = Name;
			this->Type = Type;
			CreateID();
		}

		virtual void Frame() = 0;
		virtual void Release() = 0;
		virtual void Render() = 0;
		virtual void SetResource( const IRESOURCE& Resource ) = 0;
		virtual IRESOURCE* GetResource() = 0;
};

#endif // __IRESOURCE_H__