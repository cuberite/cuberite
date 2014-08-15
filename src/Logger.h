
#pragma once


class cLogger
{
public:

	enum eLogLevel
	{
		llRegular,
		llInfo,
		llWarning,
		llError,
	};


	class cListener
	{
		public:
		virtual void Log(AString a_Message, eLogLevel a_LogLevel) = 0;

		virtual ~cListener(){}
	};

	void Log  (const char * a_Format, eLogLevel a_LogLevel, va_list a_ArgList) FORMATSTRING(2, 0);

	/** Logs the simple text message at the specified log level. */
	void LogSimple(AString a_Message, eLogLevel a_LogLevel = llRegular);

	void AttachListener(cListener * a_Listener);
	void DetachListener(cListener * a_Listener);
	
	static cLogger & GetInstance(void);
	// Must be called before calling GetInstance in a multithreaded context
	static void InitiateMultithreading();
private:

	cCriticalSection m_CriticalSection;
	std::vector<cListener *> m_LogListeners;

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




