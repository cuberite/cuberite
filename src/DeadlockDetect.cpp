
// DeadlockDetect.cpp

// Declares the cDeadlockDetect class that tries to detect deadlocks and aborts the server when it detects one

#include "Globals.h"
#include "DeadlockDetect.h"
#include "Root.h"
#include "World.h"
#include <cstdlib>





/** Number of milliseconds per cycle */
const int CYCLE_MILLISECONDS = 100;





cDeadlockDetect::cDeadlockDetect(void) :
	Super("DeadlockDetect"),
	m_IntervalSec(1000)
{
}





cDeadlockDetect::~cDeadlockDetect()
{
	// Check that all tracked CSs have been removed, report any remaining:
	cCSLock lock(m_CS);
	if (!m_TrackedCriticalSections.empty())
	{
		LOGWARNING("DeadlockDetect: Some CS objects (%u) haven't been removed from tracking", static_cast<unsigned>(m_TrackedCriticalSections.size()));
		for (const auto & tcs: m_TrackedCriticalSections)
		{
			LOGWARNING("  CS %p / %s",
				static_cast<void *>(tcs.first),
				tcs.second.c_str()
			);
		}
	}
}





bool cDeadlockDetect::Start(int a_IntervalSec)
{
	m_IntervalSec = a_IntervalSec;

	// Read the initial world data:
	cRoot::Get()->ForEachWorld([=](cWorld & a_World)
		{
			SetWorldAge(a_World.GetName(), a_World.GetWorldAge());
			return false;
		}
	);
	return Super::Start();
}





void cDeadlockDetect::TrackCriticalSection(cCriticalSection & a_CS, const AString & a_Name)
{
	cCSLock lock(m_CS);
	m_TrackedCriticalSections.emplace_back(std::make_pair(&a_CS, a_Name));
}





void cDeadlockDetect::UntrackCriticalSection(cCriticalSection & a_CS)
{
	cCSLock lock(m_CS);
	for (auto itr = m_TrackedCriticalSections.begin(), end = m_TrackedCriticalSections.end(); itr != end; ++itr)
	{
		if (itr->first == &a_CS)
		{
			m_TrackedCriticalSections.erase(itr);
			return;
		}
	}
}





void cDeadlockDetect::Execute(void)
{
	// Loop until the signal to terminate:
	while (!m_ShouldTerminate)
	{
		// Check the world ages:
		cRoot::Get()->ForEachWorld([=](cWorld & a_World)
			{
				CheckWorldAge(a_World.GetName(), a_World.GetWorldAge());
				return false;
			}
		);

		std::this_thread::sleep_for(std::chrono::milliseconds(CYCLE_MILLISECONDS));
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
		if (WorldAge.m_NumCyclesSame > (m_IntervalSec * 1000) / CYCLE_MILLISECONDS)
		{
			DeadlockDetected(a_WorldName, a_Age);
		}
	}
	else
	{
		WorldAge.m_Age = a_Age;
		WorldAge.m_NumCyclesSame = 0;
	}
}





void cDeadlockDetect::DeadlockDetected(const AString & a_WorldName, Int64 a_WorldAge)
{
	LOGERROR("Deadlock detected: world %s has been stuck at age %lld. Aborting the server.",
		a_WorldName.c_str(), static_cast<long long>(a_WorldAge)
	);
	ListTrackedCSs();
	ASSERT(!"Deadlock detected");
	abort();
}





void cDeadlockDetect::ListTrackedCSs(void)
{
	cCSLock lock(m_CS);
	for (const auto & cs: m_TrackedCriticalSections)
	{
		LOG("CS at %p, %s: RecursionCount = %d, ThreadIDHash = %04llx",
			static_cast<void *>(cs.first), cs.second.c_str(),
			cs.first->m_RecursionCount, static_cast<UInt64>(std::hash<std::thread::id>()(cs.first->m_OwningThreadID))
		);
	}
}




