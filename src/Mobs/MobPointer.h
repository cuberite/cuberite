#pragma once

/** This class allows mobs to hold pointers to other mobs/players in a safe manner.
When calling GetPointer(), it first checks if the Monster/Player being pointed to is still valid.
If not, it returns a nullptr. The returned raw pointer must be used locally and then discarded.
it MUST NOT be preserved across ticks.
*/

class cPawn;
class cWorld;
class cMobPointer
{
public:
	cMobPointer(cPawn * a_Pointer);                            // Constructor
	cMobPointer(const cMobPointer & a_MobPointer);             // Copy constructor
	cMobPointer(cMobPointer && a_MobPointer);                  // Move constructor
	cMobPointer& operator=(const cMobPointer& a_MobPointer);   // Copy assignment operator
	cMobPointer& operator=(cMobPointer&& a_MobPointer);        // Move assignment operator

	void operator=(cPawn * a_Pointer);   // set Pointer

	/** Returns the raw pointer. The returned raw pointer must
	be used locally and then discarded. it MUST NOT be preserved across ticks.
	Returns null if raw pointer is null. Returns null if mob is destroyed or moved worlds.
	Must be called at least once per tick, even if the raw pointer is not going to be used that tick. */
	cPawn * GetPointer(cWorld * a_CurrentWorld);


private:
	cPawn * m_Pointer;
} ;




