#ifndef __IRESOURCEMANAGER_H__
#define __IRESOURCEMANAGER_H__

#include "string"
#include "Rpc.h"
#include "IResource.h"

template < class T >
class IRESOURCEMANAGER
{
	public :

	public :
		IRESOURCEMANAGER();
		IRESOURCEMANAGER( const IRESOURCEMANAGER& Other );
		virtual ~IRESOURCEMANAGER();

	public :
		virtual void CreateResource( T* Resource ) = 0;
		virtual void RenderResource( T* Resource ) = 0;
		virtual void SetResource( T* Resource ) = 0;
};

#endif // __IRESOURCEMANAGER_H__