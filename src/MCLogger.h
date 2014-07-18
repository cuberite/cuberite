
#pragma once




class cLog;





class cMCLogger
{
public:
	enum eLogLevel
	{
		llRegular,
		llInfo,
		llWarning,
		llError,
	};
	// tolua_end
	
	/** Creates a logger with the default filename, "logs/LOG_<timestamp>.log" */
	cMCLogger(void);
	
	/** Creates a logger with the specified filename inside "logs" folder */
	cMCLogger(const AString & a_FileName);
	
	~cMCLogger();

	void Log  (const char * a_Format, va_list a_ArgList) FORMATSTRING(2, 0);
	void Info (const char * a_Format, va_list a_ArgList) FORMATSTRING(2, 0);
	void Warn (const char * a_Format, va_list a_ArgList) FORMATSTRING(2, 0);
	void Error(const char * a_Format, va_list a_ArgList) FORMATSTRING(2, 0);

	/** Logs the simple text message at the specified log level. */
	void LogSimple(const char * a_Text, eLogLevel a_LogLevel = llRegular);

	static cMCLogger * GetInstance();
private:
	enum eColorScheme
	{
		csRegular,
		csInfo,
		csWarning,
		csError,
	} ;
	
	cCriticalSection m_CriticalSection;
	cLog * m_Log;
	static cMCLogger * s_MCLogger;
	bool m_ShouldColorOutput;


	/// Sets the specified color scheme in the terminal (TODO: if coloring available)
	void SetColor(eColorScheme a_Scheme);
	
	/// Resets the color back to whatever is the default in the terminal
	void ResetColor(void);
	
	/// Common initialization for all constructors, creates a logfile with the specified name and assigns s_MCLogger to this
	void InitLog(const AString & a_FileName);
};





extern void LOG(const char* a_Format, ...)      FORMATSTRING(1, 2);
extern void LOGINFO(const char* a_Format, ...)  FORMATSTRING(1, 2);
extern void LOGWARN(const char* a_Format, ...)  FORMATSTRING(1, 2);
extern void LOGERROR(const char* a_Format, ...) FORMATSTRING(1, 2);





// In debug builds, translate LOGD to LOG, otherwise leave it out altogether:
#ifdef _DEBUG
	#define LOGD LOG
#else
	#define LOGD(...)
#endif  // _DEBUG





#define LOGWARNING LOGWARN




