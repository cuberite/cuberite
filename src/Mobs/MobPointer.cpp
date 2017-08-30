#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "../Entities/Pawn.h"
#include "MobPointer.h"
#include "../World.h"

cMobPointer::cMobPointer(cPawn * a_Pointer) : m_Pointer(a_Pointer)
{
	// Constructor
}





cMobPointer::cMobPointer(const cMobPointer & a_MobPointer) : m_Pointer(a_MobPointer.m_Pointer)
{
	// Copy constructor
}





cMobPointer::cMobPointer(cMobPointer && a_MobPointer)
{
	// move Constructor
	m_Pointer = a_MobPointer.m_Pointer;
	a_MobPointer.m_Pointer = nullptr;
}





cMobPointer& cMobPointer::operator=(const cMobPointer& a_MobPointer)
{
	// Copy assignment operator
	m_Pointer = a_MobPointer.m_Pointer;
	return *this;
}





cMobPointer& cMobPointer::operator=(cMobPointer&& a_MobPointer)
{
	// Move assignment operator
	m_Pointer = a_MobPointer.m_Pointer;
	a_MobPointer.m_Pointer = nullptr;
	return *this;
}





void cMobPointer::operator=(cPawn * a_Pointer)
{
	m_Pointer = a_Pointer;
}





cPawn * cMobPointer::GetPointer(cWorld * a_CurrentWorld)
{
	if (m_Pointer != nullptr)
	{
		if (!m_Pointer->IsTicking() || (m_Pointer->GetWorld() != a_CurrentWorld))
		{
			m_Pointer = nullptr;
		}
	}
	return m_Pointer;
}
