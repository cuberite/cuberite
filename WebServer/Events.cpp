
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Events.h"





cEvents::cEvents( unsigned int a_NumEvents /* = 1 */ )
	: m_NumEvents( a_NumEvents )
#ifndef _WIN32
	, m_bNamed( false )
#endif
{
    if( m_NumEvents < 1 ) m_NumEvents = 1;

#ifdef _WIN32
    m_Handle = new HANDLE[ m_NumEvents ];
	for( unsigned int i = 0; i < m_NumEvents; i++)
	{
		((HANDLE*)m_Handle)[i] = CreateEvent( 0, FALSE, FALSE, 0 );
	}
#else
    m_Handle = new sem_t*[ m_NumEvents ];
	for( unsigned int i = 0; i < m_NumEvents; i++)
	{

	    sem_t* & HandlePtr = ((sem_t**)m_Handle)[i];
	    HandlePtr = new sem_t;

        if( sem_init( HandlePtr, 0, 0 ) )
        {
            LOG("WARNING cEvents: Could not create unnamed semaphore, fallback to named.");
            m_bNamed = true;
            delete HandlePtr;   // named semaphores return their own address

            char c_Str[32];
            sprintf( c_Str, "cEvents%p", &HandlePtr );
            HandlePtr = sem_open( c_Str, O_CREAT, 777, 0 );
            if( HandlePtr == SEM_FAILED )
                LOG("ERROR: Could not create Event. (%i)", errno);
            else
                if( sem_unlink( c_Str ) != 0 )
                    LOG("ERROR: Could not unlink cEvents. (%i)", errno);
        }
	}
#endif
}





cEvents::~cEvents()
{
#ifdef _WIN32
    for( unsigned int i = 0; i < m_NumEvents; i++ )
    {
        CloseHandle( ((HANDLE*)m_Handle)[i] );
    }
    delete [] (HANDLE*)m_Handle;
#else
    for( unsigned int i = 0; i < m_NumEvents; i++ )
    {
        if( m_bNamed )
        {
            sem_t* & HandlePtr = ((sem_t**)m_Handle)[i];
            char c_Str[32];
            sprintf( c_Str, "cEvents%p", &HandlePtr );
    //        LOG("Closing event: %s", c_Str );
    //        LOG("Sem ptr: %p", HandlePtr );
	    if( sem_close( HandlePtr ) != 0 )
            {
                LOG("ERROR: Could not close cEvents. (%i)", errno);
            }
        }
        else
        {
            sem_destroy( ((sem_t**)m_Handle)[i] );
            delete ((sem_t**)m_Handle)[i];
        }
    }
    delete [] (sem_t**)m_Handle; m_Handle = 0;
#endif
}





void cEvents::Wait()
{
#ifdef _WIN32
	WaitForMultipleObjects( m_NumEvents, (HANDLE*)m_Handle, true, INFINITE );
#else
    for(unsigned int i = 0; i < m_NumEvents; i++)
    {
        if( sem_wait( ((sem_t**)m_Handle)[i] ) != 0 )
        {
            LOG("ERROR: Could not wait for cEvents. (%i)", errno);
        }
    }
#endif
}





void cEvents::Set(unsigned int a_EventNum /* = 0 */)
{
#ifdef _WIN32
	SetEvent( ((HANDLE*)m_Handle)[a_EventNum] );
#else
    if( sem_post( ((sem_t**)m_Handle)[a_EventNum] ) != 0 )
    {
        LOG("ERROR: Could not set cEvents. (%i)", errno);
    }
#endif
}




