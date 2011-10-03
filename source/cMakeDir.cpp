#include "cMakeDir.h"

#ifndef _WIN32
//#include <cstring>	// If something is missing, uncomment some of these!
//#include <cstdlib>
//#include <stdio.h>
#include <sys/stat.h>   // for mkdir
//#include <sys/types.h>
#else
#include <Windows.h>
#endif

void cMakeDir::MakeDir( const char* a_Directory )
{
#ifdef _WIN32
	SECURITY_ATTRIBUTES Attrib;
	Attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	Attrib.lpSecurityDescriptor = NULL;
	Attrib.bInheritHandle = false;
	::CreateDirectory("world", &Attrib);
#else
	mkdir("world", S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}