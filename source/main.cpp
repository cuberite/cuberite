// Mem leak detection
#include "MemoryLeak.h"

#include "cRoot.h"
#include "cMCLogger.h"

//#ifdef _WIN32
#include <exception>
//#endif

int main( int argc, char **argv )
{
	(void)argc;
	(void)argv;
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	try
	{
		cRoot Root;
		Root.Start();
	}
	catch( std::exception& e )
	{
		LOGERROR("Standard exception: %s", e.what() );
	}
	catch( ... )
	{
		LOGERROR("Unknown exception!");
	}
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}
