#ifndef __SHADER_H__
#define __SHADER_H__

#include "IResource.h"
#include "DXCommon.h"

class SHADER : public IRESOURCE
{
	public :
		SHADER();
		SHADER( const SHADER& Other );
		virtual ~SHADER();

	public :
		virtual void Frame();
		virtual void Release();
		virtual void Render();
		virtual void SetResource( const IRESOURCE* Resource );
		virtual IRESOURCE* GetResource();
};

#endif // __SHADER_H__