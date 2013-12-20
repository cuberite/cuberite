
// FastRandom.cpp

// Implements the cFastRandom class representing a fast random number generator

#include "Globals.h"
#include <time.h>
#include "FastRandom.h"





#if 0 && defined(_DEBUG)
// Self-test
// Both ints and floats are quick-tested to see if the random is calculated correctly, checking the range in ASSERTs,
// and if it performs well in terms of distribution (checked by avg, expected to be in the range midpoint
class cFastRandomTest
{
public:
	cFastRandomTest(void)
	{
		TestInts();
		TestFloats();
	}
	
	
	void TestInts(void)
	{
		printf("Testing ints...\n");
		cFastRandom rnd;
		int sum = 0;
		const int BUCKETS = 8;
		int Counts[BUCKETS];
		memset(Counts, 0, sizeof(Counts));
		const int ITER = 10000;
		for (int i = 0; i < ITER; i++)
		{
			int v = rnd.NextInt(1000);
			ASSERT(v >= 0);
			ASSERT(v < 1000);
			Counts[v % BUCKETS]++;
			sum += v;
		}
		double avg = (double)sum / ITER;
		printf("avg: %f\n", avg);
		for (int i = 0; i < BUCKETS; i++)
		{
			printf("  bucket %d: %d\n", i, Counts[i]);
		}
	}


	void TestFloats(void)
	{
		printf("Testing floats...\n");
		cFastRandom rnd;
		float sum = 0;
		const int BUCKETS = 8;
		int Counts[BUCKETS];
		memset(Counts, 0, sizeof(Counts));
		const int ITER = 10000;
		for (int i = 0; i < ITER; i++)
		{
			float v = rnd.NextFloat(1000);
			ASSERT(v >= 0);
			ASSERT(v <= 1000);
			Counts[((int)v) % BUCKETS]++;
			sum += v;
		}
		sum = sum / ITER;
		printf("avg: %f\n", sum);
		for (int i = 0; i < BUCKETS; i++)
		{
			printf("  bucket %d: %d\n", i, Counts[i]);
		}
	}
} g_Test;

#endif






int cFastRandom::m_SeedCounter = 0;





cFastRandom::cFastRandom(void) :
	m_Seed(m_SeedCounter++)
{
}





int cFastRandom::NextInt(int a_Range)
{
	ASSERT(a_Range <= 1000000);  // The random is not sufficiently linearly distributed with bigger ranges
	ASSERT(a_Range > 0);
	
	// Make the m_Counter operations as minimal as possible, to emulate atomicity
	int Counter = m_Counter++;
	
	// Use a_Range, m_Counter and m_Seed as inputs to the pseudorandom function:
	int n = a_Range + Counter * 57 + m_Seed * 57 * 57;
	n = (n << 13) ^ n;
	n = ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff);
	return ((n / 11) % a_Range);
}





int cFastRandom::NextInt(int a_Range, int a_Salt)
{
	ASSERT(a_Range <= 1000000);  // The random is not sufficiently linearly distributed with bigger ranges
	ASSERT(a_Range > 0);
	
	// Make the m_Counter operations as minimal as possible, to emulate atomicity
	int Counter = m_Counter++;
	
	// Use a_Range, a_Salt, m_Counter and m_Seed as inputs to the pseudorandom function:
	int n = a_Range + Counter * 57 + m_Seed * 57 * 57 + a_Salt * 57 * 57 * 57;
	n = (n << 13) ^ n;
	n = ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff);
	return ((n / 11) % a_Range);
}





float cFastRandom::NextFloat(float a_Range)
{
	// Make the m_Counter operations as minimal as possible, to emulate atomicity
	int Counter = m_Counter++;
	
	// Use a_Range, a_Salt, m_Counter and m_Seed as inputs to the pseudorandom function:
	int n = (int)a_Range + Counter * 57 + m_Seed * 57 * 57;
	n = (n << 13) ^ n;
	n = ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff);
	
	// Convert the integer into float with the specified range:
	return (((float)n / (float)0x7fffffff) * a_Range);
}





float cFastRandom::NextFloat(float a_Range, int a_Salt)
{
	// Make the m_Counter operations as minimal as possible, to emulate atomicity
	int Counter = m_Counter++;
	
	// Use a_Range, a_Salt, m_Counter and m_Seed as inputs to the pseudorandom function:
	int n = (int)a_Range + Counter * 57 + m_Seed * 57 * 57 + a_Salt * 57 * 57 * 57;
	n = (n << 13) ^ n;
	n = ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff);
	
	// Convert the integer into float with the specified range:
	return (((float)n / (float)0x7fffffff) * a_Range);
}




