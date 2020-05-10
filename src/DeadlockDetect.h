
// DeadlockDetect.h

// Declares the cDeadlockDetect class that tries to detect deadlocks and aborts the server when it detects one

/*
This class simply monitors each world's m_WorldAge, which is expected to grow on each tick.
If the world age doesn't grow for several seconds, it's either because the server is Super-overloaded,
or because the world tick thread hangs in a deadlock. We presume the latter and therefore kill the server.
Once we learn to write crashdumps programmatically, we should do so just before killing, to enable debugging.
*/



#pragma once

#include "OSSupport/IsThread.h"





class cDeadlockDetect:
	public cIsThread
{
	using Super = cIsThread;

public:

	cDeadlockDetect();
	virtual ~cDeadlockDetect() override;

	/** Starts the detection. Hides cIsThread's Start, because we need some initialization */
	bool Start(int a_IntervalSec);

	/** Adds the critical section for tracking.
	Tracked CSs are listed, together with ownership details, when a deadlock is detected.
	A tracked CS must be untracked before it is destroyed.
	a_Name is an arbitrary name that is listed along with the CS in the output. */
	void TrackCriticalSection(cCriticalSection & a_CS, const AString & a_Name);

	/** Removes the CS from the tracking. */
	void UntrackCriticalSection(cCriticalSection & a_CS);

protected:
	struct sWorldAge
	{
		/** Last m_WorldAge that has been detected in this world */
		Int64 m_Age;

		/** Number of cycles for which the age has been the same */
		int m_NumCyclesSame;
	} ;

	/** Maps world name -> sWorldAge */
	typedef std::map<AString, sWorldAge> WorldAges;

	/** Protects m_TrackedCriticalSections from multithreaded access. */
	cCriticalSection m_CS;

	/** CriticalSections that are tracked (their status output on deadlock).
	Protected against multithreaded access by m_CS. */
	std::vector<std::pair<cCriticalSection *, AString>> m_TrackedCriticalSections;

	WorldAges m_WorldAges;

	/** Number of secods for which the ages must be the same for the detection to trigger */
	int m_IntervalSec;


	// cIsThread overrides:
	virtual void Execute(void) override;

	/** Sets the initial world age */
	void SetWorldAge(const AString & a_WorldName, Int64 a_Age);

	/** Checks if the world's age has changed, updates the world's stats; calls DeadlockDetected() if deadlock detected */
	void CheckWorldAge(const AString & a_WorldName, Int64 a_Age);

	/** Called when a deadlock is detected in a world. Aborts the server.
	a_WorldName is the name of the world whose age has triggered the detection.
	a_WorldAge is the age (in ticks) in which the world is stuck. */
	[[noreturn]] void DeadlockDetected(const AString & a_WorldName, Int64 a_WorldAge);

	/** Outputs a listing of the tracked CSs, together with their name and state. */
	void ListTrackedCSs();
} ;




