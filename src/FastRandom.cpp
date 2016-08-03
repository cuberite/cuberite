
// FastRandom.cpp

// Implements the cFastRandom class representing a fast random number generator

#include "Globals.h"
#include "FastRandom.h"

#include <random>

#if defined (__GNUC__)
	#define ATTRIBUTE_TLS static __thread
#elif defined (_MSC_VER)
	#define ATTRIBUTE_TLS static __declspec(thread)
#else
	#error "Unknown thread local storage qualifier"
#endif

static unsigned int GetRandomSeed()
{
	ATTRIBUTE_TLS bool SeedCounterInitialized = 0;
	ATTRIBUTE_TLS unsigned int SeedCounter = 0;

	if (!SeedCounterInitialized)
	{
		std::random_device rd;
		std::uniform_int_distribution<unsigned int> dist;
		SeedCounter = dist(rd);
		SeedCounterInitialized = true;
	}
	return ++SeedCounter;
}




////////////////////////////////////////////////////////////////////////////////
// cFastRandom:





cFastRandom::cFastRandom(void) :
	m_LinearRand(GetRandomSeed())
{
}





int cFastRandom::NextInt(int a_Range)
{
	std::uniform_int_distribution<> distribution(0, a_Range - 1);
	return distribution(m_LinearRand);
}






float cFastRandom::NextFloat(float a_Range)
{
	std::uniform_real_distribution<float> distribution(0, a_Range);
	return distribution(m_LinearRand);
}






int cFastRandom::GenerateRandomInteger(int a_Begin, int a_End)
{
	std::uniform_int_distribution<> distribution(a_Begin, a_End);
	return distribution(m_LinearRand);
}





////////////////////////////////////////////////////////////////////////////////
// MTRand:

MTRand::MTRand() :
	m_MersenneRand(GetRandomSeed())
{
}





int MTRand::randInt(int a_Range)
{
	std::uniform_int_distribution<> distribution(0, a_Range);
	return distribution(m_MersenneRand);
}





int MTRand::randInt()
{
	std::uniform_int_distribution<> distribution(0, std::numeric_limits<int>::max());
	return distribution(m_MersenneRand);
}





double MTRand::rand(double a_Range)
{
	std::uniform_real_distribution<> distribution(0, a_Range);
	return distribution(m_MersenneRand);
}
