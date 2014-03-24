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

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Timer.h"






cTimer::cTimer(void)
{
	#ifdef _WIN32
		QueryPerformanceFrequency(&m_TicksPerSecond);
	#endif
}





long long cTimer::GetNowTime(void)
{
	#ifdef _WIN32
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return ((now.QuadPart * 1000) / m_TicksPerSecond.QuadPart);
	#else
		struct timeval  now;
		gettimeofday(&now, NULL);
		return (long long)now.tv_sec * 1000 + (long long)now.tv_usec / 1000;
	#endif
}




