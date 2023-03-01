#ifndef __DXMESSAGE_H__
#define __DXMESSAGE_H__

#include "DXCommon.h"
#include <queue>

class DXMESSAGE
{
	public :
		DXMESSAGE();
		DXMESSAGE( const DXMESSAGE& Other );
		~DXMESSAGE();

	public :
		bool Init();
		void Release();
		void InitPointer();
};

#endif // __DXMESSAGE_H__