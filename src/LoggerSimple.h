
// Logging free functions defined in Logger.cpp
#pragma once

// python style format specified logging

extern void vFLOG(const char * a_Format, fmt::format_args a_ArgList);
template <typename... Args>
void FLOG(const char * a_Format, const Args & ... args)
{
	vFLOG(a_Format, fmt::make_format_args(args...));
}

extern void vFLOGINFO(const char * a_Format, fmt::format_args a_ArgList);
template <typename... Args>
void FLOGINFO(const char * a_Format, const Args & ... args)
{
	vFLOGINFO(a_Format, fmt::make_format_args(args...));
}

extern void vFLOGWARNING(const char * a_Format, fmt::format_args a_ArgList);
template <typename... Args>
void FLOGWARNING(const char * a_Format, const Args & ... args)
{
	vFLOGWARNING(a_Format, fmt::make_format_args(args...));
}

extern void vFLOGERROR(const char * a_Format, fmt::format_args a_ArgList);
template <typename... Args>
void FLOGERROR(const char * a_Format, const Args & ... args)
{
	vFLOGERROR(a_Format, fmt::make_format_args(args...));
}

// printf style format specified logging (DEPRECATED)

extern void vLOG(const char * a_Format, fmt::printf_args a_ArgList);
template <typename... Args>
void LOG(const char * a_Format, const Args & ... args)
{
	vLOG(a_Format, fmt::make_printf_args(args...));
}

extern void vLOGINFO(const char * a_Format, fmt::printf_args a_ArgList);
template <typename... Args>
void LOGINFO(const char * a_Format, const Args & ... args)
{
	vLOGINFO(a_Format, fmt::make_printf_args(args...));
}

extern void vLOGWARNING(const char * a_Format, fmt::printf_args a_ArgList);
template <typename... Args>
void LOGWARNING(const char * a_Format, const Args & ... args)
{
	vLOGWARNING(a_Format, fmt::make_printf_args(args...));
}

extern void vLOGERROR(const char * a_Format, fmt::printf_args a_ArgList);
template <typename... Args>
void LOGERROR(const char * a_Format, const Args & ... args)
{
	vLOGERROR(a_Format, fmt::make_printf_args(args...));
}


// Macro variants

// In debug builds, translate LOGD to LOG, otherwise leave it out altogether:
#ifdef _DEBUG
	#define LOGD LOG
#else
	#define LOGD(...)
#endif  // _DEBUG

#define LOGWARN LOGWARNING

#ifdef _DEBUG
	#define FLOGD FLOG
#else
	#define FLOGD(...)
#endif  // _DEBUG

#define FLOGWARN FLOGWARNING
