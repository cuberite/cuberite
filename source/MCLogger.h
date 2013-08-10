
#pragma once




class cLog;





class cMCLogger														// tolua_export
{																	// tolua_export
public:																// tolua_export
	cMCLogger(void);
	cMCLogger(const AString & a_FileName);  // tolua_export
	~cMCLogger();													// tolua_export

	void Log(const char* a_Format, va_list a_ArgList);
	void Info(const char* a_Format, va_list a_ArgList);
	void Warn(const char* a_Format, va_list a_ArgList);
	void Error(const char* a_Format, va_list a_ArgList);

	void LogSimple(const char* a_Text, int a_LogType = 0 );			// tolua_export

	static cMCLogger* GetInstance();
private:
	enum eColorScheme
	{
		csRegular,
		csInfo,
		csWarning,
		csError,
	} ;
	
	/// Sets the specified color scheme in the terminal (TODO: if coloring available)
	void SetColor(eColorScheme a_Scheme);
	
	/// Resets the color back to whatever is the default in the terminal
	void ResetColor(void);
	
	cCriticalSection m_CriticalSection;
	cLog * m_Log;
	static cMCLogger * s_MCLogger;
};																	// tolua_export

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




