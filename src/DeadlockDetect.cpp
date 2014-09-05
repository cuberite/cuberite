
// DeadlockDetect.cpp

// Declares the cDeadlockDetect class that tries to detect deadlocks and aborts the server when it detects one

#include "Globals.h"
#include "DeadlockDetect.h"
#include "Root.h"
#include "World.h"
#include <cstdlib>





/// Number of milliseconds per cycle
const int CYCLE_MILLISECONDS = 100;





cDeadlockDetect::cDeadlockDetect(void) :
	super("DeadlockDetect"),
	m_IntervalSec(1000)
{
}





bool cDeadlockDetect::Start(int a_IntervalSec)
{
	m_IntervalSec = a_IntervalSec;
	
	// Read the initial world data:
	class cFillIn :
		public cWorldListCallback
	{
	public:
		cFillIn(cDeadlockDetect * a_Detect) :
			m_Detect(a_Detect)
		{
		}
		
		virtual bool Item(cWorld * a_World) override
		{
			m_Detect->SetWorldAge(a_World->GetName(), a_World->GetWorldAge());
			return false;
		}
		
	protected:
		cDeadlockDetect * m_Detect;
	} FillIn(this);
	cRoot::Get()->ForEachWorld(FillIn);
	return super::Start();
}





void cDeadlockDetect::Execute(void)
{
	// Loop until the signal to terminate:
	while (!m_ShouldTerminate)
	{
		// Check the world ages:
		class cChecker :
			public cWorldListCallback
		{
		public:
			cChecker(cDeadlockDetect * a_Detect) :
				m_Detect(a_Detect)
			{
			}
			
		protected:
			cDeadlockDetect * m_Detect;
			
			virtual bool Item(cWorld * a_World) override
			{
				m_Detect->CheckWorldAge(a_World->GetName(), a_World->GetWorldAge());
				return false;
			}
		} Checker(this);
		cRoot::Get()->ForEachWorld(Checker);
		
		cSleep::MilliSleep(CYCLE_MILLISECONDS);
	}  // while (should run)
}





void cDeadlockDetect::SetWorldAge(const AString & a_WorldName, Int64 a_Age)
{
	m_WorldAges[a_WorldName].m_Age = a_Age;
	m_WorldAges[a_WorldName].m_NumCyclesSame = 0;
}





void cDeadlockDetect::CheckWorldAge(const AString & a_WorldName, Int64 a_Age)
{
	WorldAges::iterator itr = m_WorldAges.find(a_WorldName);
	if (itr == m_WorldAges.end())
	{
		SetWorldAge(a_WorldName, a_Age);
		return;
	}

	cDeadlockDetect::sWorldAge & WorldAge = itr->second;

	if (WorldAge.m_Age == a_Age)
	{
		WorldAge.m_NumCyclesSame += 1;
		if (WorldAge.m_NumCyclesSame > (1000 * m_IntervalSec) / CYCLE_MILLISECONDS)
		{
			DeadlockDetected();
		}
	}
	else
	{
		WorldAge.m_Age = a_Age;
		WorldAge.m_NumCyclesSame = 0;
	}
}





void cDeadlockDetect::DeadlockDetected(void)
{
	LOGERROR("Deadlock detected, aborting the server");
	ASSERT(!"Deadlock detected");
	abort();
}




