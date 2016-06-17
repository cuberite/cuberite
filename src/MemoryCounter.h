#pragma once
#include <atomic>
#include <cstdlib>  // for Size_t

class cMemoryCounter
{

public:
	cMemoryCounter();

	/** Increments the approximated chunk RAM consumption counter by a_Increment. Unit is bytes. */
	void IncrementApproximateChunkRAM(size_t a_Increment);

	/** Decrements the approximated chunk RAM consumption counter by a_Decrement. Unit is bytes. */
	void DecrementApproximateChunkRAM(size_t a_Decrement);

	/** Increments the counter of the amount of unused chunks our world has by one. */
	void IncrementCanUnloadCount();

	/** Decrements the conter of the amount of unused chunks our world has by one. */
	void DecrementCanUnloadCount();

	/** Returns the value of the number of chunks that can be unloaded by our world. Thread safe. */
	size_t GetCanUnloadCount();

	/** Returns the approximated chunk RAM consumption in MiB. Thread safe. */
	size_t GetApproximateChunkRAM();

private:
	std::atomic<uint64_t> m_ApproximateChunkRamSize;
	std::atomic<size_t> m_CanUnloadCount;
};
