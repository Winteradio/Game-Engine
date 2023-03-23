#ifndef __TIMER_H__
#define __TIMER_H__

#include "Utils.h"

class Timer
{
	public :
		static std::string Get()
		{
			SYSTEMTIME sysTime;
			FILETIME fTime, fTempTime;

			char Time[MAX_LENGTH];
			GetSystemTimeAsFileTime( &fTempTime );
			FileTimeToLocalFileTime( &fTempTime, &fTime );
			FileTimeToSystemTime( &fTime, &sysTime );

			sprintf( Time, "%d:%d:%d.%d ", sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds );

			std::string Result = Time;

			return Result;
		}
};

#endif // __TIMER_H__