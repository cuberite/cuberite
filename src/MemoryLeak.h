/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
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
