
#pragma once





class cLog
{
private:
	FILE * m_File;
	static cLog * s_Log;

public:
	cLog(const AString & a_FileName);
	~cLog();
	void Log(const char * a_Format, va_list argList) FORMATSTRING(2, 0);
	void Log(const char * a_Format, ...) FORMATSTRING(2, 3);
	// tolua_begin
	void SimpleLog(const char * a_String);
	void OpenLog(const char * a_FileName);
	void CloseLog();
	void ClearLog();
	static cLog* GetInstance();
};





