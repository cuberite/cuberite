
// FastRandomTest.cpp

// Tests the randomness of cFastRandom

#include "Globals.h"
#include "../TestHelpers.h"
#include "FastRandom.h"





static void TestInts(void)
{
	cFastRandom rnd;
	int sum = 0;
	const int BUCKETS = 8;
	int Counts[BUCKETS] = {0};
	const int ITER = 10000;
	for (int i = 0; i < ITER; i++)
	{
		int v = rnd.RandInt(1000);
		TEST_GREATER_THAN_OR_EQUAL(v, 0);
		TEST_LESS_THAN_OR_EQUAL(v, 1000);
		Counts[v % BUCKETS]++;
		sum += v;
	}
	double avg = static_cast<double>(sum) / ITER;
	LOG("avg: %f", avg);
	for (int i = 0; i < BUCKETS; i++)
	{
		LOG("  bucket %d: %d", i, Counts[i]);
	}
}





static void TestFloats(void)
{
	cFastRandom rnd;
	float sum = 0;
	const int BUCKETS = 8;
	int Counts[BUCKETS] = {0};
	const int ITER = 10000;
	for (int i = 0; i < ITER; i++)
	{
		float v = rnd.RandReal(1000.0f);
		TEST_GREATER_THAN_OR_EQUAL(v, 0);
		TEST_LESS_THAN_OR_EQUAL(v, 1000);
		Counts[static_cast<int>(v) % BUCKETS]++;
		sum += v;
	}
	sum = sum / ITER;
	LOG("avg: %f", sum);
	for (int i = 0; i < BUCKETS; i++)
	{
		LOG("  bucket %d: %d", i, Counts[i]);
	}
}





/** Checks whether re-creating the cFastRandom class produces the same initial number over and over (#2935) */
static void TestReCreation(void)
{
	const int ITER = 10000;
	int lastVal = 0;
	int numSame = 0;
	int maxNumSame = 0;
	for (int i = 0; i < ITER; ++i)
	{
		cFastRandom rnd;
		int val = rnd.RandInt(9);
		if (val == lastVal)
		{
			numSame += 1;
		}
		else
		{
			if (numSame > maxNumSame)
			{
				maxNumSame = numSame;
			}
			numSame = 0;
			lastVal = val;
		}
	}
	if (numSame > maxNumSame)
	{
		maxNumSame = numSame;
	}
	LOG("Out of %d creations, there was a chain of at most %d same numbers generated.", ITER, maxNumSame);
}





IMPLEMENT_TEST_MAIN("FastRandom",
	TestInts();
	TestFloats();
	TestReCreation();
)
