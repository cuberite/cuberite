
// Logging free functions defined in Logger.cpp
#pragma once

extern void LOG(const char * a_Format, fmt::ArgList a_ArgList);
FMT_VARIADIC(void, LOG, const char *);

extern void LOGINFO(const char * a_Format, fmt::ArgList a_ArgList);
FMT_VARIADIC(void, LOGINFO, const char *);

extern void LOGWARNING(const char * a_Format, fmt::ArgList a_ArgList);
FMT_VARIADIC(void, LOGWARNING, const char *);

extern void LOGERROR(const char * a_Format, fmt::ArgList a_ArgList);
FMT_VARIADIC(void, LOGERROR, const char *);


// In debug builds, translate LOGD to LOG, otherwise leave it out altogether:
#ifdef _DEBUG
	#define LOGD LOG
#else
	#define LOGD(...)
#endif  // _DEBUG

#define LOGWARN LOGWARNING
