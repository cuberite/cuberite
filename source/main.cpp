// Mem leak detection
#include "MemoryLeak.h"

#include "cRoot.h"
#include "cMCLogger.h"

#include <exception> //std::exception
#include <stdio.h>   //printf
#include <csignal>   //std::signal
#include <stdlib.h>  //exit()

#include "SquirrelBindings.h"
#if USE_SQUIRREL
#pragma warning(disable:4100;disable:4127;disable:4510;disable:4610;disable:4244;disable:4512) // Getting A LOT of these warnings from SqPlus
#include <sqplus/sqplus.h>
#pragma warning(default:4100;default:4127;default:4510;default:4610;default:4244;default:4512)
#endif

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

#ifndef _DEBUG
	std::signal(SIGSEGV, ShowCrashReport);
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

#if USE_SQUIRREL
	SquirrelVM::Shutdown();
#endif

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}
