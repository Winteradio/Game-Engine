#ifndef __SCENE_H__
#define __SCENE_H__

#include "IResource.h"
#include <vector>

class SCENE : public IRESOURCE
{
	public :
		SCENE();
		SCENE( const SCENE& Other);
		virtual ~SCENE();

	public :
		virtual void Frame();
		virtual void Release();
		virtual void Render();
		virtual void SetResource( const IRESOURCE* Resource );
		virtual IRESOURCE* GetResource();

	private :
		std::vector< IRESOURCE* > SceneData;
};

#endif // __SCENE_H__