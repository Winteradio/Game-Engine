#ifndef __DXLOG_H__
#define __DXLOG_H__

#include <stdarg.h>
#include <string>
#include <cstring>
#include <vector>

class DXLOG
{
	public :
		static DXLOG* pDXLog = nullptr;

	public :
		static DXLOG*& GetDXLog();
		static void Set( const char* Type, const char* Str, ... );
		static void Info();
		static void Warn();
		static void Debug();
		static void Error();
		static void Trace();
		static vector<string>& GetLogs();

		void Clear();
		void Save();

	private :
		DXLOG();
		DXLOG( const DXLOG& Other );
		~DXLOG();

		int MaxLength = 512;
		std::vector<std::string> Message;
};

#endif // __DXLOG_H__