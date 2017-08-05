
// FastRandom.cpp


#include "Globals.h"
#include "FastRandom.h"

#if defined (__GNUC__)
	#define ATTRIBUTE_TLS static __thread
#elif defined (_MSC_VER)
	#define ATTRIBUTE_TLS static __declspec(thread)
#else
	#define ATTRIBUTE_TLS thread_local
#endif





MTRand & GetRandomProvider()
{
	// Some compilers don't support thread_local for non-POD types, this is purely a work around for that restriction.
	// There should be minimal overhead for the non-initializing case and all thread's instances are deleted properly.
	ATTRIBUTE_TLS MTRand * LocalPtr = nullptr;
	if (LocalPtr == nullptr)
	{
		// This list allows deletion of elements as if they had static storage duration
		static std::mutex CSDeleteList;
		static std::list<std::unique_ptr<MTRand>> DeleteList;

		cRandomDeviceSeeder seeder;
		auto NewInstance = cpp14::make_unique<MTRand>(seeder);
		auto TempPtr = NewInstance.get();

		std::lock_guard<std::mutex> Lock(CSDeleteList);
		DeleteList.push_front(std::move(NewInstance));
		LocalPtr = TempPtr;  // Set after push_back so LocalPtr won't dangle if it throws
	}
	return *LocalPtr;
}





UInt32 Detail::GetRandomSeed()
{
	ATTRIBUTE_TLS bool SeedCounterInitialized = false;
	ATTRIBUTE_TLS UInt32 SeedCounter = 0;

	if (!SeedCounterInitialized)
	{
		std::random_device rd;
		std::uniform_int_distribution<UInt32> dist;
		SeedCounter = dist(rd);
		SeedCounterInitialized = true;
	}
	return ++SeedCounter;
}
