#ifndef __MESH_H__
#define __MESH_H__

#include "IResource.h"
#include "DXCommon.h"

class MESH : public IRESOURCE
{
	public :
		MESH();
		MESH( const MESH& Other );
		virtual ~MESH();

	public :
		virtual void Frame();
		virtual void Release();
		virtual void Render();
		virtual void SetResource( const IRESOURCE* Resource );
		virtual IRESOURCE* GetResource();
};

#endif // __MESH_H__