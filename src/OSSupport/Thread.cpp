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





// When in MSVC, the debugger provides "thread naming" by catching special exceptions. Interface here:
#ifdef _MSC_VER
//
// Usage: SetThreadName (-1, "MainThread");
//
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // must be 0x1000
	LPCSTR szName; // pointer to name (in user addr space)
	DWORD dwThreadID; // thread ID (-1=caller thread)
	DWORD dwFlags; // reserved for future use, must be zero
} THREADNAME_INFO;

void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = szThreadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try
	{
		RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info );
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}
#endif  // _MSC_VER





cThread::cThread( ThreadFunc a_ThreadFunction, void* a_Param, const char* a_ThreadName /* = 0 */ )
	: m_ThreadFunction( a_ThreadFunction )
	, m_Param( a_Param )
	, m_Event( new cEvent() )
	, m_StopEvent( 0 )
{
	if( a_ThreadName )
	{
		m_ThreadName.assign(a_ThreadName);
	}
}





cThread::~cThread()
{
	delete m_Event;
	
	if( m_StopEvent )
	{
		m_StopEvent->Wait();
		delete m_StopEvent;
	}
}





void cThread::Start( bool a_bWaitOnDelete /* = true */ )
{
	if( a_bWaitOnDelete )
		m_StopEvent = new cEvent();

#ifndef _WIN32
	pthread_t SndThread;
	if( pthread_create( &SndThread, NULL, MyThread, this) )
		LOGERROR("ERROR: Could not create thread!");
#else
	DWORD ThreadID = 0;
	HANDLE hThread = CreateThread(	0 // security
		,0 // stack size
		, (LPTHREAD_START_ROUTINE) MyThread // function name
		,this // parameters
		,0 // flags
		,&ThreadID ); // thread id
	CloseHandle( hThread );

	#ifdef _MSC_VER
	if (!m_ThreadName.empty())
	{
		SetThreadName(ThreadID, m_ThreadName.c_str());
	}
	#endif  // _MSC_VER
#endif

	// Wait until thread has actually been created
	m_Event->Wait();
}





#ifdef _WIN32
unsigned long cThread::MyThread(void* a_Param )
#else
void *cThread::MyThread( void *a_Param )
#endif
{
	cThread* self = (cThread*)a_Param;
	cEvent* StopEvent = self->m_StopEvent;

	ThreadFunc* ThreadFunction = self->m_ThreadFunction;
	void* ThreadParam = self->m_Param;

	// Set event to let other thread know this thread has been created and it's safe to delete the cThread object
	self->m_Event->Set();

	ThreadFunction( ThreadParam );

	if( StopEvent ) StopEvent->Set();
	return 0;
}
