// Mem leak detection
#include "MemoryLeak.h"

#include "cRoot.h"
#include "cMCLogger.h"

#include <exception> //std::exception
#include <stdio.h>   //printf
#include <csignal>   //std::signal
#include <stdlib.h>  //exit()

void ShowCrashReport(int) 
{
	std::signal(SIGSEGV, SIG_DFL);

	printf("\n\nMCServer has crashed!\n");

	exit(-1);
}

int main( int argc, char **argv )
{
	(void)argc;
	(void)argv;
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	std::signal(SIGSEGV, ShowCrashReport);

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
