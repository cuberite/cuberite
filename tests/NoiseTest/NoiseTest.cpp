
// NoiseTest.cpp

// Implements the main app entrypoint

#include "Globals.h"
#include <time.h>
#include "Noise.h"





void SaveValues(NOISE_DATATYPE * a_Values, const AString & a_FileName)
{
	cFile f;
	if (!f.Open(a_FileName, cFile::fmWrite))
	{
		LOGWARNING("Cannot write file %s", a_FileName.c_str());
		return;
	}
	for (int y = 0; y < 256; y++)
	{
		unsigned char val[256];
		for (int x = 0; x < 256; x++)
		{
			val[x] = std::min(255, std::max(0, (int)(256 * a_Values[x + 256 * y])));
		}
		f.Write(val, 256);
	}
}





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
	LOG("cCubicNoise generating 1000 * 256x256 values took %d ticks (%.02f sec)", Ticks, (double)Ticks / CLOCKS_PER_SEC);
	
	// Save the results into a file for visual comparison:
	SaveValues(Values, "NoiseCubic.raw");
	
	return Ticks;
}





clock_t TestOldNoise(void)
{
	cNoise Noise(0);
	NOISE_DATATYPE Values[256 * 256];

	// Do a speed test:
	clock_t Begin = clock();
	for (int i = 0; i < 1000; i++)
	{
		for (int y = 0; y < 256; y++)
		{
			float fy = (float)y / 10;
			for (int x = 0; x < 256; x++)
			{
				Values[x + 256 * y] = Noise.CubicNoise2D((float)x / 10, fy);
			}  // for x
		}  // for y
	}
	clock_t Ticks = clock() - Begin;
	LOG("cNoise generating 1000 * 256x256 values took %d ticks (%.02f sec)", Ticks, (double)Ticks / CLOCKS_PER_SEC);
	
	// Save the results into a file for visual comparison:
	SaveValues(Values, "NoiseOld.raw");
	
	return Ticks;
}





int main(int argc, char * argv[])
{
	new cMCLogger();  // Create a logger (will set itself as the main instance
	
	clock_t NewTicks = TestCubicNoise();
	clock_t OldTicks = TestOldNoise();
	LOG("New method is %.02fx faster", (double)OldTicks / NewTicks);
	LOG("Press Enter to quit program");
	getchar();
}
