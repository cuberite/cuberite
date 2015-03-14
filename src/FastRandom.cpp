
// FastRandom.cpp

// Implements the cFastRandom class representing a fast random number generator

#include "Globals.h"
#include "FastRandom.h"





////////////////////////////////////////////////////////////////////////////////
// cFastRandom:

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





cFastRandom::cFastRandom(void) :
	m_LinearRand(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()))
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
	m_MersenneRand(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()))
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




