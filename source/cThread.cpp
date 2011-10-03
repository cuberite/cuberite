#ifndef _WIN32
#include <cstring>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include "cThread.h"
#include "cEvent.h"
#include "cMCLogger.h"

cThread::cThread( ThreadFunc a_ThreadFunction, void* a_Param )
	: m_ThreadFunction( a_ThreadFunction )
	, m_Param( a_Param )
	, m_Event( new cEvent() )
	, m_StopEvent( 0 )
{
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
	HANDLE hThread = CreateThread(	0 // security
		,0 // stack size
		, (LPTHREAD_START_ROUTINE) MyThread // function name
		,this // parameters
		,0 // flags
		,0 ); // thread id
	CloseHandle( hThread );
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
