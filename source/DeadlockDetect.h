
// DeadlockDetect.h

// Declares the cDeadlockDetect class that tries to detect deadlocks and aborts the server when it detects one

/*
This class simply monitors each world's m_WorldAge, which is expected to grow on each tick.
If the world age doesn't grow for several seconds, it's either because the server is super-overloaded,
or because the world tick thread hangs in a deadlock. We presume the latter and therefore kill the server.
Once we learn to write crashdumps programmatically, we should do so just before killing, to enable debugging.
*/



#pragma once

#include "OSSupport/IsThread.h"





class cDeadlockDetect :
	public cIsThread
{
	typedef cIsThread super;
	
public:
	cDeadlockDetect(void);
	
	/// Starts the detection. Hides cIsThread's Start, because we need some initialization
	bool Start(void);
	
	/// Stops the detection. Hides cIsThread's Stop, because we need to signal m_EvtTerminate
	void Stop(void);
	
protected:
	struct sWorldAge
	{
		/// Last m_WorldAge that has been detected in this world
		Int64 m_Age;
		
		/// Number of cycles for which the age has been the same
		int m_NumCyclesSame;
	} ;
	
	/// Maps world name -> sWorldAge
	typedef std::map<AString, sWorldAge> WorldAges;
	
	WorldAges m_WorldAges;
	
	cEvent m_EvtTerminate;
	
	
	// cIsThread overrides:
	virtual void Execute(void) override;
	
	/// Sets the initial world age
	void SetWorldAge(const AString & a_WorldName, Int64 a_Age);
	
	/// Checks if the world's age has changed, updates the world's stats; calls DeadlockDetected() if deadlock detected
	void CheckWorldAge(const AString & a_WorldName, Int64 a_Age);
	
	/// Called when a deadlock is detected. Aborts the server.
	void DeadlockDetected(void);
} ;




