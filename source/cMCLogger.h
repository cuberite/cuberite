
#pragma once




class cLog;





class cMCLogger														//tolua_export
{																	//tolua_export
private:
#ifdef _WIN32
	typedef char* va_list;
#endif
public:																//tolua_export
	cMCLogger();
	cMCLogger( char* a_File );										//tolua_export
	~cMCLogger();													//tolua_export

	void Log(const char* a_Format, va_list a_ArgList);
	void Info(const char* a_Format, va_list a_ArgList);
	void Warn(const char* a_Format, va_list a_ArgList);
	void Error(const char* a_Format, va_list a_ArgList);

	void LogSimple(const char* a_Text, int a_LogType = 0 );			//tolua_export

	static cMCLogger* GetInstance();
private:
	void SetColor( unsigned char a_Color );

	cCriticalSection m_CriticalSection;
	cLog* m_Log;
	static cMCLogger* s_MCLogger;
};																	//tolua_export

extern void LOG(const char* a_Format, ...);
extern void LOGINFO(const char* a_Format, ...);
extern void LOGWARN(const char* a_Format, ...);
extern void LOGERROR(const char* a_Format, ...);





// In debug builds, translate LOGD to LOG, otherwise leave it out altogether:
#ifdef _DEBUG
	#define LOGD LOG
#else
	#define LOGD(...)
#endif  // _DEBUG





#define LOGWARNING LOGWARN




