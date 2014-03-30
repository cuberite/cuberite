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

// Timer.h

// Declares the cTimer class representing an OS-independent of retrieving current time with msec accuracy





#pragma once





class cTimer
{
public:
	cTimer(void);

	// Returns the current time expressed in milliseconds
	long long GetNowTime(void);
private:

	#ifdef _WIN32
		LARGE_INTEGER m_TicksPerSecond;
	#endif
} ;




