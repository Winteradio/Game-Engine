#ifndef __DXLOG_H__
#define __DXLOG_H__

#include "DXLog.h"

DXLOG::DXLOG()
{
	
}

DXLOG::DXLOG( const DXLOG& Other )
{
	
}

DXLOG::~DXLOG()
{
	
}

DXLOG*& DXLOG::GetDXLog()
{
	if ( pDXLog == nullptr )
	{
		pDXLog = new DXLOG;
	}

	return pDXLog;
}

void DXLOG::Set( const char* Type, const char* Str, ... )
{
	char Buff[ MaxLength ] = "";

	std::string Temp = Type;

	va_list pArg;
	va_start( pArg, Str );
	vsprintf( Buff + strlen( Buff ), Str, pArg );
	va_end( pArg );

	Temp += Buff;

	Message.push_back( Temp );
}

void DXLOG::Info( const )

