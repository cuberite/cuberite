
#include "Globals.h"

#include "Promise.h"

cPromise * cPromise::WaitFor(cPromise * a_Promise) 
{
	return new cCombinedPromise(this, a_Promise);
}

cPromise * cPromise::CancelOn(volatile bool& cancelation) 
{
	return new cCancelablePromise(this, cancelation);
}

void cPromise::Wait()
{
	while(!IsCompleted()){}; //busywait best we can do until waitany
}


cCombinedPromise::cCombinedPromise(cPromise* a_left, cPromise* a_right) : 
	cPromise(), 
	m_left(a_left), 
	m_right(a_right)
{
}

cCombinedPromise::~cCombinedPromise()
{
}

bool cCombinedPromise::IsCompleted()
{
	return m_left->IsCompleted() || m_right->IsCompleted();
}

cCancelablePromise::cCancelablePromise(cPromise* a_wrapped, volatile bool& a_cancel) : 
	cPromise(), 
	m_cancel(a_cancel),
	m_wrapped(a_wrapped)
{
}

cCancelablePromise::~cCancelablePromise ()
{
}

bool cCancelablePromise::IsCompleted() 
{
	return m_cancel || m_wrapped->IsCompleted();
}


