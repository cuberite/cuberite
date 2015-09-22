
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

	class cAttachment
	{
		public:

		cAttachment(cAttachment && a_other)
			: m_listener(a_other.m_listener)
		{
		}

		~cAttachment()
		{
			cLogger::GetInstance().DetachListener(m_listener);
		}

		private:

		cListener * m_listener;

		friend class cLogger;

		cAttachment(cListener * a_listener) : m_listener(a_listener) {}
	};

	void Log  (const char * a_Format, eLogLevel a_LogLevel, va_list a_ArgList) FORMATSTRING(2, 0);

	/** Logs the simple text message at the specified log level. */
	void LogSimple(AString a_Message, eLogLevel a_LogLevel = llRegular);

	cAttachment AttachListener(std::unique_ptr<cListener> a_Listener);
	
	static cLogger & GetInstance(void);
	// Must be called before calling GetInstance in a multithreaded context
	static void InitiateMultithreading();
private:

	cCriticalSection m_CriticalSection;
	std::vector<std::unique_ptr<cListener>> m_LogListeners;

	void DetachListener(cListener * a_Listener);

};




// These declarations are duplicated in globals.h
extern void LOG       (const char * a_Format, ...) FORMATSTRING(1, 2);
extern void LOGINFO   (const char * a_Format, ...) FORMATSTRING(1, 2);
extern void LOGWARNING(const char * a_Format, ...) FORMATSTRING(1, 2);
extern void LOGERROR  (const char * a_Format, ...) FORMATSTRING(1, 2);



