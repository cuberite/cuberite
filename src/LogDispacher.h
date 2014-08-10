
#pragma once



class cLog;


namespace Logger
{

	enum eLogLevel
	{
		llRegular,
		llInfo,
		llWarning,
		llError,
	};

	class cLogDispacher;

	// Must be called before calling GetInstance in a multithreaded context
	void InitiateMultithreading();
	
	cLogDispacher & GetInstance(void);
	
	class cLoggerListener
	{
		public:
		virtual void Log(AString a_Message, eLogLevel a_LogLevel) = 0;
		
		virtual ~cLoggerListener(){}
	};

	class cLogDispacher
	{
	public:

		void Log  (const char * a_Format, Logger::eLogLevel a_LogLevel, va_list a_ArgList) FORMATSTRING(2, 0);

		/** Logs the simple text message at the specified log level. */
		void LogSimple(AString a_Message, Logger::eLogLevel a_LogLevel = Logger::llRegular);
	
		void AttachListener(Logger::cLoggerListener * a_Listener);
		void DetachListener(Logger::cLoggerListener * a_Listener);

	private:
	
		cCriticalSection m_CriticalSection;
		std::vector<Logger::cLoggerListener *> m_LogListeners;
	
	};

}






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




