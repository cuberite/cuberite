
// NoiseTest.cpp

// Implements the main app entrypoint

#include "Globals.h"
#include "Noise/Noise.h"
#include "OldNoise.h"




using namespace std::chrono;





/** Generates the same area by both noises and compares the resulting values. */
static void CompareNoises(void)
{
	static const size_t SIZE_X = 4;
	static const size_t SIZE_Y = 4;
	cCubicNoise    NewCubic(0);
	cOldCubicNoise OldCubic(0);
	NOISE_DATATYPE NewVals[SIZE_X * SIZE_Y], OldVals[SIZE_X * SIZE_Y];
	NewCubic.Generate2D(NewVals, SIZE_X, SIZE_Y, 0, 1, 0, 1);
	OldCubic.Generate2D(OldVals, SIZE_X, SIZE_Y, 0, 1, 0, 1);
	if (memcmp(NewVals, OldVals, sizeof(NewVals)) == 0)
	{
		printf("The noises generate the same values.\n");
	}
	else
	{
		printf("The noises generate DIFFERENT values!\n");
	}
}





static double TestCubicNoise(void)
{
	cCubicNoise Cubic(0);
	NOISE_DATATYPE Values[256 * 256];
	
	// Do a speed test:
	auto Begin = high_resolution_clock::now();
	NOISE_DATATYPE v = 0;
	for (int i = 0; i < 1000; i++)
	{
		NOISE_DATATYPE StartX = static_cast<NOISE_DATATYPE>(25.6 * i);
		NOISE_DATATYPE EndX   = static_cast<NOISE_DATATYPE>(25.6 + StartX);
		NOISE_DATATYPE StartY = static_cast<NOISE_DATATYPE>(25.6 * i);
		NOISE_DATATYPE EndY   = static_cast<NOISE_DATATYPE>(25.6 + StartY);
		Cubic.Generate2D(Values, 256, 256, StartX, EndX, StartY, EndY);
		v += Values[i];  // Force the optimizer not to remove this calculation
	}
	double Ticks = static_cast<double>(duration_cast<milliseconds>(high_resolution_clock::now() - Begin).count());
	printf("cCubicNoise generating 1000 * 256x256 values took %.02f ticks (%.02f sec)\n(final value %f)\n", Ticks, Ticks / 1000, v);
	
	return Ticks;
}





static double TestOldNoise(void)
{
	cOldCubicNoise Cubic(0);
	NOISE_DATATYPE Values[256 * 256];

	// Do a speed test:
	auto Begin = high_resolution_clock::now();
	NOISE_DATATYPE v = 0;
	for (int i = 0; i < 1000; i++)
	{
		NOISE_DATATYPE StartX = static_cast<NOISE_DATATYPE>(25.6 * i);
		NOISE_DATATYPE EndX   = static_cast<NOISE_DATATYPE>(25.6 + StartX);
		NOISE_DATATYPE StartY = static_cast<NOISE_DATATYPE>(25.6 * i);
		NOISE_DATATYPE EndY   = static_cast<NOISE_DATATYPE>(25.6 + StartY);
		Cubic.Generate2D(Values, 256, 256, StartX, EndX, StartY, EndY);
		v += Values[i];  // Force the optimizer not to remove this calculation
	}
	double Ticks = static_cast<double>(duration_cast<milliseconds>(high_resolution_clock::now() - Begin).count());
	printf("cOldCubicNoise generating 1000 * 256x256 values took %.02f ticks (%.02f sec)\n(final value %f)\n", Ticks, Ticks / 1000, v);
	
	return Ticks;
}





int main(int argc, char * argv[])
{
	// new cMCLogger();  // Create a logger (will set itself as the main instance
	
	CompareNoises();
	auto NewTicks = TestCubicNoise();
	auto OldTicks = TestOldNoise();
	printf("New method is %.02fx faster\n", OldTicks / NewTicks);
	printf("Press Enter to quit program\n");
	getchar();
}




