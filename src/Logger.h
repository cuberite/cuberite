
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

		cAttachment() : m_listener(nullptr) {}
		cAttachment(cAttachment && a_other)
			: m_listener(a_other.m_listener)
		{
			a_other.m_listener = nullptr;
		}

		~cAttachment()
		{
			if (m_listener != nullptr)
			{
				cLogger::GetInstance().DetachListener(m_listener);
			}
		}

		cAttachment & operator=(cAttachment && a_other)
		{
			m_listener = a_other.m_listener;
			a_other.m_listener = nullptr;
			return *this;
		}

		private:

		cListener * m_listener;

		friend class cLogger;

		cAttachment(cListener * a_listener) : m_listener(a_listener) {}
	};

	/** Log a message formatted with a printf style formatting string. */
	void LogPrintf(const char * a_Format, eLogLevel a_LogLevel, fmt::ArgList a_ArgList);
	FMT_VARIADIC(void, LogPrintf, const char *, eLogLevel)

	/** Log a message formatted with a python style formatting string. */
	void LogFormat(const char * a_Format, eLogLevel a_LogLevel, fmt::ArgList a_ArgList);
	FMT_VARIADIC(void, LogFormat, const char *, eLogLevel)

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



