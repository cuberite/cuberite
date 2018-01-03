// NoiseSpeedTest.cpp

// Implements the main app entrypoint

/*
This program compares the performance of the highly-optimized noise implementation in Cuberite, and the Simplex noise.
Since the Simplex noise is not yet implemented in Cuberite, an own implementation is provided.
Also, the performance difference between using a float and double as datatype is measured, by using a templatized Simplex noise.

The testing is done on a usage of the generator that is typical for the Cuberite's terrain generator: generate a 3D array of numbers with
not much variance in the coords. The exact sizes and coord ranges were adapted from the cNoise3DComposable generator.
*/

#include "Globals.h"
#include "Noise/Noise.h"
#include "Noise/InterpolNoise.h"
#include "SimplexNoise.h"





/** The sizes of the interpolated noise that are calculated: */
static const int SIZE_X = 33;
static const int SIZE_Y = 5;
static const int SIZE_Z = 5;





static void measureClassicNoise(int a_NumIterations)
{
	cInterp5DegNoise noise(1);
	NOISE_DATATYPE total = 0;
	auto timeStart = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < a_NumIterations; ++i)
	{
		NOISE_DATATYPE out[SIZE_X * SIZE_Y * SIZE_Z];
		int blockX = i * 16;
		int blockZ = i * 16;
		NOISE_DATATYPE startX = 0;
		NOISE_DATATYPE endX = 257 / 80.0f;
		NOISE_DATATYPE startY = blockX / 40.0f;
		NOISE_DATATYPE endY = (blockX + 16) / 40.0f;
		NOISE_DATATYPE startZ = blockZ / 40.0f;
		NOISE_DATATYPE endZ = (blockZ + 16) / 40.0f;
		noise.Generate3D(out, SIZE_X, SIZE_Y, SIZE_Z, startX, endX, startY, endY, startZ, endZ);
		total += out[0];  // Do not let the optimizer optimize the whole calculation away
	}
	auto timeEnd = std::chrono::high_resolution_clock::now();
	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart);
	printf("Classic noise took %d milliseconds, returned total %f\n", static_cast<int>(msec.count()), total);
}





/** Calculates the specified number of iterations of the Simplex noise.
a_TypeStr is a string representing the DATATYPE (for logging purposes). */
template<typename DATATYPE> static void measureSimplexNoise(int a_NumIterations, const char * a_TypeStr)
{
	cSimplexNoise<DATATYPE> noise(1);
	DATATYPE total = 0;
	auto timeStart = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < a_NumIterations; ++i)
	{
		DATATYPE out[SIZE_X * SIZE_Y * SIZE_Z];
		int blockX = i * 16;
		int blockZ = i * 16;
		DATATYPE startX = 0;
		DATATYPE endX = 257 / 80.0f;
		DATATYPE startY = blockX / 40.0f;
		DATATYPE endY = (blockX + 16) / 40.0f;
		DATATYPE startZ = blockZ / 40.0f;
		DATATYPE endZ = (blockZ + 16) / 40.0f;
		noise.Generate3D(out, SIZE_X, SIZE_Y, SIZE_Z, startX, endX, startY, endY, startZ, endZ);
		total += out[0];  // Do not let the optimizer optimize the whole calculation away
	}
	auto timeEnd = std::chrono::high_resolution_clock::now();
	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart);
	printf("SimplexNoise<%s> took %d milliseconds, returned total %f\n", a_TypeStr, static_cast<int>(msec.count()), total);
}





int main(int argc, char ** argv)
{
	int numIterations = 10000;
	if (argc > 1)
	{
		numIterations = std::atoi(argv[1]);
		if (numIterations < 10)
		{
			printf("Invalid number of iterations, using 1000 instead\n");
			numIterations = 1000;
		}
	}

	// Perform each test twice, to account for cache-warmup:
	measureClassicNoise(numIterations);
	measureClassicNoise(numIterations);
	measureSimplexNoise<float>(numIterations, "float");
	measureSimplexNoise<float>(numIterations, "float");
	measureSimplexNoise<double>(numIterations, "double");
	measureSimplexNoise<double>(numIterations, "double");

	// If build on Windows using MSVC, wait for a keypress before ending:
	#ifdef _MSC_VER
		getchar();
	#endif

	return 0;
}
