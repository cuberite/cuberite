
// NoiseTest.cpp

// Implements the main app entrypoint

#include "Globals.h"
#include "Noise/Noise.h"
#include "OldNoise.h"




using namespace std::chrono;





static double TestCubicNoise(void)
{
	cCubicNoise Cubic(0);
	NOISE_DATATYPE Values[256 * 256];
	
	// Do a speed test:
	auto Begin = high_resolution_clock::now();
	for (int i = 0; i < 1000; i++)
	{
		Cubic.Generate2D(Values, 256, 256, 0, (NOISE_DATATYPE)25.6, 0, (NOISE_DATATYPE)25.6);
	}
	double Ticks = static_cast<double>(duration_cast<milliseconds>(high_resolution_clock::now() - Begin).count());
	printf("cCubicNoise generating 1000 * 256x256 values took %.02f ticks (%.02f sec)\n", Ticks, Ticks / 1000);
	
	return Ticks;
}





static double TestOldNoise(void)
{
	cOldCubicNoise Cubic(0);
	NOISE_DATATYPE Values[256 * 256];

	// Do a speed test:
	auto Begin = high_resolution_clock::now();
	for (int i = 0; i < 1000; i++)
	{
		Cubic.Generate2D(Values, 256, 256, 0, (NOISE_DATATYPE)25.6, 0, (NOISE_DATATYPE)25.6);
	}
	double Ticks = static_cast<double>(duration_cast<milliseconds>(high_resolution_clock::now() - Begin).count());
	printf("cOldCubicNoise generating 1000 * 256x256 values took %.02f ticks (%.02f sec)\n", Ticks, Ticks / 1000);
	
	return Ticks;
}





int main(int argc, char * argv[])
{
	// new cMCLogger();  // Create a logger (will set itself as the main instance
	
	auto NewTicks = TestCubicNoise();
	auto OldTicks = TestOldNoise();
	printf("New method is %.02fx faster\n", OldTicks / NewTicks);
	printf("Press Enter to quit program\n");
	getchar();
}




