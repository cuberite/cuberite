
// NetworkLookup.cpp

// Implements the cNetworkLookup class representing an executor for asynchronous lookup tasks


#include "Globals.h"
#include "NetworkLookup.h"




cNetworkLookup::cNetworkLookup() :
	cIsThread("NetworkLookup")
{
}





cNetworkLookup::~cNetworkLookup()
{
	Stop();
}





void cNetworkLookup::ScheduleLookup(std::function<void()> a_Lookup)
{
	m_WorkQueue.EnqueueItem(std::move(a_Lookup));
}





void cNetworkLookup::Stop()
{
	m_ShouldTerminate = true;
	m_WorkQueue.Clear();
	m_WorkQueue.EnqueueItem([](){});  // Dummy work to wake up the thread
	cIsThread::Stop();
}





void cNetworkLookup::Execute()
{
	while (!m_ShouldTerminate)
	{
		// Execute the next task in the queue
		auto Work = m_WorkQueue.DequeueItem();
		Work();
	}
}



