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

// Event.h

// Interfaces to the cEvent object representing an OS-specific synchronization primitive that can be waited-for
// Implemented as an Event on Win and as a 1-semaphore on *nix





#pragma once
#ifndef CEVENT_H_INCLUDED
#define CEVENT_H_INCLUDED





class cEvent
{
public:
	cEvent(void);
	~cEvent();

	void Wait(void);
	void Set (void);
	
private:

	#ifdef _WIN32
	HANDLE m_Event;
	#else
	sem_t * m_Event;
	bool    m_bIsNamed;
	#endif
} ;






#endif  // CEVENT_H_INCLUDED




