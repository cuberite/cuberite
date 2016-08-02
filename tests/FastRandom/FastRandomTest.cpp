
// FastRandomTest.cpp

// Tests the randomness of cFastRandom

#include "Globals.h"
#include "FastRandom.h"


static void TestInts(void)
{
	cFastRandom rnd;
	int sum = 0;
	const int BUCKETS = 8;
	int Counts[BUCKETS];
	memset(Counts, 0, sizeof(Counts));
	const int ITER = 10000;
	for (int i = 0; i < ITER; i++)
	{
		int v = rnd.NextInt(1000);
		assert_test(v >= 0);
		assert_test(v < 1000);
		Counts[v % BUCKETS]++;
		sum += v;
	}
	double avg = static_cast<double>(sum) / ITER;
	printf("avg: %f\n", avg);
	for (int i = 0; i < BUCKETS; i++)
	{
		printf("  bucket %d: %d\n", i, Counts[i]);
	}
}


static void TestFloats(void)
{
	cFastRandom rnd;
	float sum = 0;
	const int BUCKETS = 8;
	int Counts[BUCKETS];
	memset(Counts, 0, sizeof(Counts));
	const int ITER = 10000;
	for (int i = 0; i < ITER; i++)
	{
		float v = rnd.NextFloat(1000);
		assert_test(v >= 0);
		assert_test(v <= 1000);
		Counts[static_cast<int>(v) % BUCKETS]++;
		sum += v;
	}
	sum = sum / ITER;
	printf("avg: %f\n", sum);
	for (int i = 0; i < BUCKETS; i++)
	{
		printf("  bucket %d: %d\n", i, Counts[i]);
	}
}


int main(void)
{
	LOGD("FastRandom Test started");

	LOGD("Testing ints");
	TestInts();

	LOGD("Testing floats");
	TestFloats();

	LOG("FastRandom test finished");
}
