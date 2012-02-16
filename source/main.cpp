
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cRoot.h"

#include <exception> //std::exception
#include <csignal>   //std::signal
#include <stdlib.h>  //exit()

#include "SquirrelBindings.h"
#if USE_SQUIRREL
	#pragma warning(push)
	#pragma warning(disable:4100;disable:4127;disable:4510;disable:4610;disable:4244;disable:4512) // Getting A LOT of these warnings from SqPlus
	#include <sqplus/sqplus.h>
	#pragma warning(pop)
#endif




/// If defined, a thorough leak finder will be used (debug MSVC only); leaks will be output to the Output window
#define ENABLE_LEAK_FINDER





#if defined(_MSC_VER) && defined(_DEBUG) && defined(ENABLE_LEAK_FINDER)
	#define XML_LEAK_FINDER
	#pragma warning(push)
	#pragma warning(disable:4100)
	#include "LeakFinder.h"
	#pragma warning(pop)
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
	
	#if defined(_MSC_VER) && defined(_DEBUG) && defined(ENABLE_LEAK_FINDER)
	InitLeakFinder();
	#endif
	
	#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	
	// _X: The simple built-in CRT leak finder - simply break when allocating the Nth block ({N} is listed in the leak output)
	// Only useful when the leak is in the same sequence all the time
	// _CrtSetBreakAlloc(85950);
	
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

	#if defined(_MSC_VER) && defined(_DEBUG) && defined(ENABLE_LEAK_FINDER)
	DeinitLeakFinder();
	#endif
	
	return 0;
}




