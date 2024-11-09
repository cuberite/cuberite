
// FastRandom.cpp


#include "Globals.h"
#include "FastRandom.h"





MTRand & GetRandomProvider()
{
	thread_local MTRand Random = []
		{
			cRandomDeviceSeeder Seeder;
			return MTRand(Seeder);
		}();
	return Random;
}





UInt32 Detail::GetRandomSeed()
{
	thread_local UInt32 SeedCounter = []
		{
			std::random_device rd;
			std::uniform_int_distribution<UInt32> dist;
			return dist(rd);
		}();
	return ++SeedCounter;
}
