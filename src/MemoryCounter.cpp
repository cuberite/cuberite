#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "MemoryCounter.h"





cMemoryCounter::cMemoryCounter() :
	m_ApproximateChunkRamSize(0),
	m_CanUnloadCount(0)
{

}





void cMemoryCounter::IncrementApproximateChunkRAM(size_t a_Increment)
{
	m_ApproximateChunkRamSize += a_Increment;
}





void cMemoryCounter::DecrementApproximateChunkRAM(size_t a_Decrement)
{
	ASSERT(m_ApproximateChunkRamSize >= a_Decrement);
	m_ApproximateChunkRamSize -= a_Decrement;
}




/** Increments the counter of the amount of unused chunks our world has by one. */
void cMemoryCounter::IncrementCanUnloadCount()
{
	++m_CanUnloadCount;
}





void cMemoryCounter::DecrementCanUnloadCount()
{
	--m_CanUnloadCount;
}





size_t cMemoryCounter::GetCanUnloadCount()
{
	return m_CanUnloadCount;
}





size_t cMemoryCounter::GetApproximateChunkRAM()
{
	return static_cast<size_t>(m_ApproximateChunkRamSize / 1048576);  // Cast to MiB
}
