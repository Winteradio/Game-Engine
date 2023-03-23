#ifndef __DXBASE_H__
#define __DXBASE_H__

#include "DXCommon.h"

class DXBASE
{
	public :
		DXBASE();
		DXBASE( const DXBASE& Other );
		virtual ~DXBASE();
		DXLOG& GetLog();

	private :
		static DXLOG* Log;
};

DXLOG* DXBASE::Log = nullptr;

#endif // __DXBASE_H__