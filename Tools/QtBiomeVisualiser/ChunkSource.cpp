#include "Globals.h"
#include "ChunkSource.h"
#include "Generating/BioGen.h"





/** Map for converting biome values to colors. Initialized from biomeColors[]. */
static uchar biomeToColor[256 * 4];

/** Map for converting biome values to colors. Used to initialize biomeToColor[].*/
static struct
{
	EMCSBiome m_Biome;
	uchar m_Color[3];
} biomeColors[] =
{
	{ biOcean,                { 0x00, 0x00, 0x70 }, },
	{ biPlains,               { 0x8d, 0xb3, 0x60 }, },
	{ biDesert,               { 0xfa, 0x94, 0x18 }, },
	{ biExtremeHills,         { 0x60, 0x60, 0x60 }, },
	{ biForest,               { 0x05, 0x66, 0x21 }, },
	{ biTaiga,                { 0x0b, 0x66, 0x59 }, },
	{ biSwampland,            { 0x2f, 0xff, 0xda }, },
	{ biRiver,                { 0x30, 0x30, 0xaf }, },
	{ biHell,                 { 0x7f, 0x00, 0x00 }, },
	{ biSky,                  { 0x00, 0x7f, 0xff }, },
	{ biFrozenOcean,          { 0xa0, 0xa0, 0xdf }, },
	{ biFrozenRiver,          { 0xa0, 0xa0, 0xff }, },
	{ biIcePlains,            { 0xff, 0xff, 0xff }, },
	{ biIceMountains,         { 0xa0, 0xa0, 0xa0 }, },
	{ biMushroomIsland,       { 0xff, 0x00, 0xff }, },
	{ biMushroomShore,        { 0xa0, 0x00, 0xff }, },
	{ biBeach,                { 0xfa, 0xde, 0x55 }, },
	{ biDesertHills,          { 0xd2, 0x5f, 0x12 }, },
	{ biForestHills,          { 0x22, 0x55, 0x1c }, },
	{ biTaigaHills,           { 0x16, 0x39, 0x33 }, },
	{ biExtremeHillsEdge,     { 0x7f, 0x8f, 0x7f }, },
	{ biJungle,               { 0x53, 0x7b, 0x09 }, },
	{ biJungleHills,          { 0x2c, 0x42, 0x05 }, },

	{ biJungleEdge,           { 0x62, 0x8b, 0x17 }, },
	{ biDeepOcean,            { 0x00, 0x00, 0x30 },	},
	{ biStoneBeach,           { 0xa2, 0xa2, 0x84 },	},
	{ biColdBeach,            { 0xfa, 0xf0, 0xc0 },	},
	{ biBirchForest,          { 0x30, 0x74, 0x44 },	},
	{ biBirchForestHills,     { 0x1f, 0x5f, 0x32 },	},
	{ biRoofedForest,         { 0x40, 0x51, 0x1a },	},
	{ biColdTaiga,            { 0x31, 0x55, 0x4a },	},
	{ biColdTaigaHills,       { 0x59, 0x7d, 0x72 },	},
	{ biMegaTaiga,            { 0x59, 0x66, 0x51 },	},
	{ biMegaTaigaHills,       { 0x59, 0x66, 0x59 },	},
	{ biExtremeHillsPlus,     { 0x50, 0x70, 0x50 },	},
	{ biSavanna,              { 0xbd, 0xb2, 0x5f },	},
	{ biSavannaPlateau,       { 0xa7, 0x9d, 0x64 },	},
	{ biMesa,                 { 0xd9, 0x45, 0x15 },	},
	{ biMesaPlateauF,         { 0xb0, 0x97, 0x65 },	},
	{ biMesaPlateau,          { 0xca, 0x8c, 0x65 },	},

	// M variants:
	{ biSunflowerPlains,      { 0xb5, 0xdb, 0x88 }, },
	{ biDesertM,              { 0xff, 0xbc, 0x40 }, },
	{ biExtremeHillsM,        { 0x88, 0x88, 0x88 }, },
	{ biFlowerForest,         { 0x2d, 0x8e, 0x49 }, },
	{ biTaigaM,               { 0x33, 0x8e, 0x81 }, },
	{ biSwamplandM,           { 0x07, 0xf9, 0xb2 }, },
	{ biIcePlainsSpikes,      { 0xb4, 0xdc, 0xdc }, },
	{ biJungleM,              { 0x7b, 0xa3, 0x31 }, },
	{ biJungleEdgeM,          { 0x62, 0x8b, 0x17 }, },
	{ biBirchForestM,         { 0x58, 0x9c, 0x6c }, },
	{ biBirchForestHillsM,    { 0x47, 0x87, 0x5a }, },
	{ biRoofedForestM,        { 0x68, 0x79, 0x42 }, },
	{ biColdTaigaM,           { 0x24, 0x3f, 0x36 }, },
	{ biMegaSpruceTaiga,      { 0x45, 0x4f, 0x3e }, },
	{ biMegaSpruceTaigaHills, { 0x45, 0x4f, 0x4e }, },
	{ biExtremeHillsPlusM,    { 0x78, 0x98, 0x78 }, },
	{ biSavannaM,             { 0xe5, 0xda, 0x87 }, },
	{ biSavannaPlateauM,      { 0xa7, 0x9d, 0x74 }, },
	{ biMesaBryce,            { 0xff, 0x6d, 0x3d }, },
	{ biMesaPlateauFM,        { 0xd8, 0xbf, 0x8d }, },
	{ biMesaPlateauM,         { 0xf2, 0xb4, 0x8d }, },
} ;





static class BiomeColorsInitializer
{
public:
	BiomeColorsInitializer(void)
	{
		// Reset all colors to gray:
		for (size_t i = 0; i < ARRAYCOUNT(biomeToColor); i++)
		{
			biomeToColor[i] = 0x7f;
		}

		// Set known biomes to their colors:
		for (size_t i = 0; i < ARRAYCOUNT(biomeColors); i++)
		{
			uchar * color = &biomeToColor[4 * biomeColors[i].m_Biome];
			color[0] = biomeColors[i].m_Color[0];
			color[1] = biomeColors[i].m_Color[1];
			color[2] = biomeColors[i].m_Color[2];
			color[3] = 0xff;
		}
	}
} biomeColorInitializer;





/** Converts biomes in an array into the chunk image data. */
static void biomesToImage(cChunkDef::BiomeMap & a_Biomes, Chunk::Image & a_Image)
{
	// Make sure the two arrays are of the same size, compile-time.
	// Note that a_Image is actually 4 items per pixel, so the array is 4 times bigger:
	static const char Check1[4 * ARRAYCOUNT(a_Biomes) - ARRAYCOUNT(a_Image)      + 1];
	static const char Check2[ARRAYCOUNT(a_Image)      - 4 * ARRAYCOUNT(a_Biomes) + 1];

	// Convert the biomes into color:
	for (size_t i = 0; i < ARRAYCOUNT(a_Biomes); i++)
	{
		a_Image[4 * i + 0] = biomeToColor[4 * a_Biomes[i] + 0];
		a_Image[4 * i + 1] = biomeToColor[4 * a_Biomes[i] + 1];
		a_Image[4 * i + 2] = biomeToColor[4 * a_Biomes[i] + 2];
		a_Image[4 * i + 3] = biomeToColor[4 * a_Biomes[i] + 3];
	}
}





////////////////////////////////////////////////////////////////////////////////
// BioGenSource:

BioGenSource::BioGenSource(cBiomeGen * a_BiomeGen) :
	m_BiomeGen(a_BiomeGen)
{
}





void BioGenSource::getChunkBiomes(int a_ChunkX, int a_ChunkZ, ChunkPtr a_DestChunk)
{
	// TODO: To make use of multicore machines, we need multiple copies of the biomegen
	// Right now we have only one, so we can let only one thread use it (hence the mutex)
	QMutexLocker lock(&m_Mtx);
	cChunkDef::BiomeMap biomes;
	m_BiomeGen->GenBiomes(a_ChunkX, a_ChunkZ, biomes);
	Chunk::Image img;
	biomesToImage(biomes, img);
	a_DestChunk->setImage(img);
}




