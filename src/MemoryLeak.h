#pragma once

#ifdef _WIN32
	#ifdef _DEBUG
		// Enable the CRT debugging features:
		#define _CRTDBG_MAP_ALLOC
		#include <stdlib.h>
		#include <crtdbg.h>
		
		// This works only in MSVC 2010+:
		#if _MSC_VER >= 1600
			// Map the new operator
			#ifndef DEBUG_NEW
				#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
				#define new DEBUG_NEW
			#endif  // _CRTDBG_MAP_ALLOC
		#endif  // _MSC_VER
	#endif  // _DEBUG
#endif  // _WIN32
