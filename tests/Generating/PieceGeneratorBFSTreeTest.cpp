// PieceGeneratorBFSTreeTest.cpp

// Implements the test for the cPieceGeneratorBFSTree class

/*
Unlike most tests, this is actually not meant as much for unit-testing, but more like performance-testing.
Compile this project in Release mode, then run it in folder that has NetherFort.cubeset prefabs, too, using
a higher number of repetitions (each repetition takes time on the order of a second); investigate the
runtime performance with a profiler.

The syntax to execute this test:
PieceGeneratorBFSTreeTest [<numRepetitions>]
numRepetitions is the number of NetherFort structures to be generated (default = 1)
*/





#include "Globals.h"
#include "Generating/PrefabPiecePool.h"
#include "Generating/PieceGeneratorBFSTree.h"





static int test(int a_NumRepetitions)
{
	// Load the piece pool:
	cPrefabPiecePool pool;
	if (!pool.LoadFromFile("NetherFort.cubeset", true))
	{
		LOGERROR("Cannot load the NetherFort cubeset. Please make sure that the NetherFort.cubeset file is present in the current dir and that it is a valid .cubeset file.");
		return -1;
	}
	auto & generatorParams = pool.GetAllMetadata();
	auto maxDepth = GetStringMapInteger<int>(generatorParams, "MaxDepth", 5);

	// Perform repeated test of the generator:
	auto start = std::chrono::high_resolution_clock::now();
	cPieceGeneratorBFSTree pg(pool, 1);
	for (int i = 0; i < a_NumRepetitions; ++i)
	{
		cPlacedPieces placedPieces;
		pg.PlacePieces(i * 512, 0, maxDepth, placedPieces);
		placedPieces.clear();
	}  // for i - numRepetitions
	auto end = std::chrono::high_resolution_clock::now();

	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	LOG("Performed %d repetition(s), took %.3f seconds", a_NumRepetitions, static_cast<double>(dur) / 1000);
	return 0;
}





int main(int argc, char * argv[])
{
	LOGD("Starting the PieceGeneratorBFSTree test.");

	// Parse the cmdline parameters:
	int numRepetitions = 1;
	for (int i = 1; i < argc; ++i)
	{
		int rep = atoi(argv[i]);
		if (rep > 0)
		{
			numRepetitions = rep;
		}
	}
	LOGD("Performing %d repetitions", numRepetitions);

	auto res = test(numRepetitions);
	if (res != 0)
	{
		LOGD("Test failed.");
		return res;
	}

	LOGD("The PieceGeneratorBFSTree test competed successfully.");
	return 0;
}




