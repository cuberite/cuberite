
// NetworkLookup.h

// Declares the cNetworkLookup class representing an executor for asynchronous lookup tasks

#pragma once

#include <functional>

#include "IsThread.h"
#include "Queue.h"





class cNetworkLookup :
	public cIsThread
{
public:

	cNetworkLookup();
	virtual ~cNetworkLookup() override;

	/** Schedule a lookup task for execution. */
	void ScheduleLookup(std::function<void()> a_Lookup);

	/** Cancels any scheduled lookups and joins the lookup thread. */
	void Stop();

protected:

	/** Process the queue until the thread is stopped. */
	virtual void Execute() override final;

private:

	/** The queue of lookup tasks waiting to be executed. */
	cQueue<std::function<void()>> m_WorkQueue;
};


