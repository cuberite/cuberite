
// AnvilStats.cpp

// Implements the main app entrypoint

#include "Globals.h"
#include "Processor.h"
#include "Statistics.h"
#include "BiomeMap.h"
#include "HeightMap.h"
#include "HeightBiomeMap.h"
#include "ChunkExtract.h"
#include "SpringStats.h"





int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		LOG("Usage: %s <method number> [<world folder>]", argv[0]);
		LOG("Available methods:");
		LOG("  0 - statistics");
		LOG("  1 - biome map");
		LOG("  2 - height map");
		LOG("  3 - extract chunks");
		LOG("  4 - count lava- and water- springs");
		LOG("  5 - biome and height map");
		LOG("\nNo method number present, aborting.");
		return -1;
	}

	AString WorldFolder;
	if (argc > 2)
	{
		WorldFolder = argv[2];
	}
	else
	{
		WorldFolder = "." + cFile::PathSeparator;
	}

	cCallbackFactory * Factory = NULL;
	switch (atol(argv[1]))
	{
		case 0: Factory = new cStatisticsFactory;                break;
		case 1: Factory = new cBiomeMapFactory;                  break;
		case 2: Factory = new cHeightMapFactory;                 break;
		case 3: Factory = new cChunkExtractFactory(WorldFolder); break;
		case 4: Factory = new cSpringStatsFactory;               break;
		case 5: Factory = new cHeightBiomeMapFactory;            break;
		default:
		{
			LOG("Unknown method \"%s\", aborting.", argv[1]);
			return -2;
		}
	}
	cProcessor Processor;
	Processor.ProcessWorld(WorldFolder, *Factory);

	LOG("Processing finished");

	delete Factory;

	LOG("Done");
}
