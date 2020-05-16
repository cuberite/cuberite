
// Logging free functions defined in Logger.cpp
#pragma once

enum class eLogLevel
{
	Regular,
	Info,
	Warning,
	Error,
};

namespace Logger
{

extern void LogFormat(
	std::string_view a_Format, eLogLevel a_LogLevel, fmt::format_args a_ArgList
);
extern void LogPrintf(
	std::string_view a_Format, eLogLevel a_LogLevel, fmt::printf_args a_ArgList
);
extern void LogSimple(std::string_view a_Message, eLogLevel a_LogLevel);

}  // namespace Logger

// python style format specified logging

template <typename... Args>
void FLOG(std::string_view a_Format, const Args & ... args)
{
	Logger::LogFormat(a_Format, eLogLevel::Regular, fmt::make_format_args(args...));
}

template <typename... Args>
void FLOGINFO(std::string_view a_Format, const Args & ... args)
{
	Logger::LogFormat(a_Format, eLogLevel::Info, fmt::make_format_args(args...));
}

template <typename... Args>
void FLOGWARNING(std::string_view a_Format, const Args & ... args)
{
	Logger::LogFormat(a_Format, eLogLevel::Warning, fmt::make_format_args(args...));
}

template <typename... Args>
void FLOGERROR(std::string_view a_Format, const Args & ... args)
{
	Logger::LogFormat(a_Format, eLogLevel::Error, fmt::make_format_args(args...));
}

// printf style format specified logging (DEPRECATED)

template <typename... Args>
void LOG(std::string_view a_Format, const Args & ... args)
{
	Logger::LogPrintf(a_Format, eLogLevel::Regular, fmt::make_printf_args(args...));
}

template <typename... Args>
void LOGINFO(std::string_view a_Format, const Args & ... args)
{
	Logger::LogPrintf(a_Format, eLogLevel::Info, fmt::make_printf_args(args...));
}

template <typename... Args>
void LOGWARNING(std::string_view a_Format, const Args & ... args)
{
	Logger::LogPrintf(a_Format, eLogLevel::Warning, fmt::make_printf_args(args...));
}

template <typename... Args>
void LOGERROR(std::string_view a_Format, const Args & ... args)
{
	Logger::LogPrintf(a_Format, eLogLevel::Error, fmt::make_printf_args(args...));
}


// Macro variants

// In debug builds, translate LOGD to LOG, otherwise leave it out altogether:
#if defined(_DEBUG) || defined(TEST_GLOBALS)
	#define LOGD LOG
#else
	#define LOGD(...)
#endif  // _DEBUG

#define LOGWARN LOGWARNING

#if defined(_DEBUG) || defined(TEST_GLOBALS)
	#define FLOGD FLOG
#else
	#define FLOGD(...)
#endif  // _DEBUG

#define FLOGWARN FLOGWARNING
