
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cMakeDir.h"





void cMakeDir::MakeDir( const char* a_Directory )
{
#ifdef _WIN32
	SECURITY_ATTRIBUTES Attrib;
	Attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	Attrib.lpSecurityDescriptor = NULL;
	Attrib.bInheritHandle = false;
	::CreateDirectory(a_Directory, &Attrib);
#else
	mkdir(a_Directory, S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}




