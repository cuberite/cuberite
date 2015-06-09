
// NoiseTest.cpp

// Implements the main app entrypoint

#include "Globals.h"
#include <time.h>
#include "Noise/Noise.h"
#include "OldNoise.h"




clock_t TestCubicNoise(void)
{
	cCubicNoise Cubic(0);
	NOISE_DATATYPE Values[256 * 256];
	
	// Do a speed test:
	clock_t Begin = clock();
	for (int i = 0; i < 1000; i++)
	{
		Cubic.Generate2D(Values, 256, 256, 0, (NOISE_DATATYPE)25.6, 0, (NOISE_DATATYPE)25.6);
	}
	clock_t Ticks = clock() - Begin;
	printf("cCubicNoise generating 1000 * 256x256 values took %d ticks (%.02f sec)\n", Ticks, (double)Ticks / CLOCKS_PER_SEC);
	
	return Ticks;
}





clock_t TestOldNoise(void)
{
	cOldCubicNoise Cubic(0);
	NOISE_DATATYPE Values[256 * 256];

	// Do a speed test:
	clock_t Begin = clock();
	for (int i = 0; i < 1000; i++)
	{
		Cubic.Generate2D(Values, 256, 256, 0, (NOISE_DATATYPE)25.6, 0, (NOISE_DATATYPE)25.6);
	}
	clock_t Ticks = clock() - Begin;
	printf("cOldCubicNoise generating 1000 * 256x256 values took %d ticks (%.02f sec)\n", Ticks, (double)Ticks / CLOCKS_PER_SEC);
	
	return Ticks;
}





int main(int argc, char * argv[])
{
	// new cMCLogger();  // Create a logger (will set itself as the main instance
	
	clock_t NewTicks = TestCubicNoise();
	clock_t OldTicks = TestOldNoise();
	printf("New method is %.02fx faster\n", (double)OldTicks / NewTicks);
	printf("Press Enter to quit program\n");
	getchar();
}
