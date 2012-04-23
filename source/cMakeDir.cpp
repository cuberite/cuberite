
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cMakeDir.h"





void cMakeDir::MakeDir(const AString & a_Directory)
{
#ifdef _WIN32
	SECURITY_ATTRIBUTES Attrib;
	Attrib.nLength = sizeof(SECURITY_ATTRIBUTES);
	Attrib.lpSecurityDescriptor = NULL;
	Attrib.bInheritHandle = false;
	::CreateDirectory(a_Directory.c_str(), &Attrib);
#else
	mkdir(a_Directory.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}




