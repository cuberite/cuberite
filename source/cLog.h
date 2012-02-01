#pragma once

#include "MemoryLeak.h"
#include "FileDefine.h"

#ifndef _WIN32
	#include <stdarg.h>
#endif





class cLog
{	// tolua_export
private:
	FILE * m_File;
	static cLog * s_Log;

public:
	cLog(const AString & a_FileName);
	~cLog();
	void Log(const char* a_Format, va_list argList );
	void Log(const char* a_Format, ...);
	//tolua_begin
	void SimpleLog(const char* a_String);
	void OpenLog( const char* a_FileName );
	void CloseLog();
	void ClearLog();
	static cLog* GetInstance();
};
//tolua_end




