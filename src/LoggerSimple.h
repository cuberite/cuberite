
// Logging free functions defined in Logger.cpp
#pragma once

// python style format specified logging

extern void FLOG(const char * a_Format, fmt::ArgList a_ArgList);
FMT_VARIADIC(void, FLOG, const char *)

extern void FLOGINFO(const char * a_Format, fmt::ArgList a_ArgList);
FMT_VARIADIC(void, FLOGINFO, const char *)

extern void FLOGWARNING(const char * a_Format, fmt::ArgList a_ArgList);
FMT_VARIADIC(void, FLOGWARNING, const char *)

extern void FLOGERROR(const char * a_Format, fmt::ArgList a_ArgList);
FMT_VARIADIC(void, FLOGERROR, const char *)

// printf style format specified logging (DEPRECATED)

extern void LOG(const char * a_Format, fmt::ArgList a_ArgList);
FMT_VARIADIC(void, LOG, const char *)

extern void LOGINFO(const char * a_Format, fmt::ArgList a_ArgList);
FMT_VARIADIC(void, LOGINFO, const char *)

extern void LOGWARNING(const char * a_Format, fmt::ArgList a_ArgList);
FMT_VARIADIC(void, LOGWARNING, const char *)

extern void LOGERROR(const char * a_Format, fmt::ArgList a_ArgList);
FMT_VARIADIC(void, LOGERROR, const char *)


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
