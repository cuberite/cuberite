
// GrownBiomeGenVisualiser.cpp

// Implements the main app entrypoint

#include "Globals.h"
#include <iostream>
#include <fstream>
#include <random>
#define PROT_INT_BUFFER_SIZE (130 * 130)
#include "Generating/ProtIntGen.h"

#include "fmt/printf.h"




typedef int Color[3];  // Color is an array of 3 ints




// Forward declarations, needed for GCC and Clang:
void outputBitmapFile(
	const AString & a_FileName,
	unsigned a_ImageSizeX, unsigned a_ImageSizeY,
	const int * a_ColorIndices,
	unsigned a_IndicesSizeX, unsigned a_IndicesSizeY,
	const Color * a_Colors,
	size_t a_NumColors
);
void initializeBiomeColors(void);
void generateZoomLevels(int a_Seed);
void generateSmoothLevels(int a_Seed);
void generateExamples(int a_Seed);






/** Color palette used for algorithm examples.
No relevance to biomes whatsoever. */
static const Color spectrumColors[] =
{
	{0, 0, 0},
	{255, 0, 0},
	{0, 255, 0},
	{0, 0, 255},
	{255, 255, 0},
	{255, 0, 255},
	{0, 255, 255},
};





/** Color palette used for displaying biome groups. */
static const Color biomeGroupColors[] =
{
	/* bgOcean     */  {0x00, 0x00, 0x70},
	/* bgDesert    */  {0xfa, 0x94, 0x18},
	/* bgTemperate */  {0x05, 0x66, 0x21},
	/* bgMountains */  {0x60, 0x60, 0x60},
	/* bgIce       */  {0xa0, 0xa0, 0xff},
};





/** Color palette used for outputting biome images.
Initialized from biomeColorMap[] in initializeBiomeColors(). */
static Color biomeColors[255];




/** Map of biome -> color, used to initialize biomeColorMap[]. */
static const struct
{
	EMCSBiome biome;
	Color color;
}
biomeColorMap[] =
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
	{ biDeepOcean,            { 0x00, 0x00, 0x30 }, },
	{ biStoneBeach,           { 0xa2, 0xa2, 0x84 }, },
	{ biColdBeach,            { 0xfa, 0xf0, 0xc0 }, },
	{ biBirchForest,          { 0x30, 0x74, 0x44 }, },
	{ biBirchForestHills,     { 0x1f, 0x5f, 0x32 }, },
	{ biRoofedForest,         { 0x40, 0x51, 0x1a }, },
	{ biColdTaiga,            { 0x31, 0x55, 0x4a }, },
	{ biColdTaigaHills,       { 0x59, 0x7d, 0x72 }, },
	{ biMegaTaiga,            { 0x59, 0x66, 0x51 }, },
	{ biMegaTaigaHills,       { 0x59, 0x66, 0x59 }, },
	{ biExtremeHillsPlus,     { 0x50, 0x70, 0x50 }, },
	{ biSavanna,              { 0xbd, 0xb2, 0x5f }, },
	{ biSavannaPlateau,       { 0xa7, 0x9d, 0x64 }, },
	{ biMesa,                 { 0xd9, 0x45, 0x15 }, },
	{ biMesaPlateauF,         { 0xb0, 0x97, 0x65 }, },
	{ biMesaPlateau,          { 0xca, 0x8c, 0x65 }, },

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
};





template <typename ... Args>
void log(const char * a_Fmt, const Args & ... a_Args)
{
	fmt::printf(a_Fmt, a_Args...);
	putchar('\n');
	fflush(stdout);
}





void outputBitmapFile(
	const AString & a_FileName,
	unsigned a_ImageSizeX, unsigned a_ImageSizeY,
	const int * a_ColorIndices,
	unsigned a_IndicesSizeX, unsigned a_IndicesSizeY,
	const Color * a_Colors,
	size_t a_NumColors
)
{
	std::ofstream f(a_FileName, std::ios::out | std::ios::binary);
	if (!f.good())
	{
		log("Cannot open file %s for writing. Skipping.", a_FileName.c_str());
		return;
	}
	f << "P3\r\n" << a_ImageSizeX << " " << a_ImageSizeY << "\r\n255\r\n";
	unsigned oldIndY = 0;
	for (unsigned y = 0; y < a_ImageSizeY; y++)
	{
		unsigned indY = y * a_IndicesSizeY / a_ImageSizeY;
		if (oldIndY != indY)
		{
			// Output a horizontal divider line:
			for (unsigned x = 0; x < a_ImageSizeX; x++)
			{
				f << "128 128 128  ";
			}
			f << "\r\n";
			oldIndY = indY;
			continue;
		}
		unsigned oldIndX = 0;
		for (unsigned x = 0; x < a_ImageSizeX; x++)
		{
			unsigned indX = x * a_IndicesSizeX / a_ImageSizeX;
			if (indX == oldIndX)
			{
				auto & color = a_Colors[a_ColorIndices[indX + a_IndicesSizeX * indY]];
				f << color[0] << " " << color[1] << " " << color[2] << "  ";
			}
			else
			{
				// vertical divider line:
				f << "128 128 128  ";
			}
			oldIndX = indX;
		}
		f << "\r\n";
	}
}





/** Initializes biomeColors[] using the biomeColorMap[]. */
void initializeBiomeColors(void)
{
	// Initialize all colors to red, so that anything unassigned is visible:
	for (size_t i = 0; i < ARRAYCOUNT(biomeColors); i++)
	{
		auto & color = biomeColors[i];
		color[0] = 0;
		color[1] = 0xff;
		color[2] = 0;
	}

	// Initialize per-biome:
	for (size_t i = 0; i < ARRAYCOUNT(biomeColorMap); i++)
	{
		auto & dst = biomeColors[biomeColorMap[i].biome];
		const auto & src = biomeColorMap[i].color;
		dst[0] = src[0];
		dst[1] = src[1];
		dst[2] = src[2];
	}
}





/** Generates a series of images showing the "zoom" effect of the IntGen zoom operation.
Each image is the same size, the integer arrays are scaled to fit the image. */
void generateZoomLevels(int a_Seed)
{
	log("Generating zoom levels...");
	const unsigned NumImages = 7;  ///< Number of images to generate. The more images, the larger they will be
	const unsigned maxArrSize = (1 << NumImages) + 1;
	for (unsigned i = 1; i <= NumImages; i++)
	{
		unsigned arrSize = (1 << i) + 1;  // Dimensions of the actually generated array
		ASSERT(arrSize <= maxArrSize);
		int workspace[maxArrSize * maxArrSize];  // Workspace for the generated array

		// Chain the zoom operation as many times as the image number:
		std::shared_ptr<cProtIntGen> gen = std::make_shared<cProtIntGenChoice>(a_Seed, static_cast<int>(ARRAYCOUNT(spectrumColors) + 1));
		for (unsigned j = 1; j < i; j++)
		{
			gen = std::make_shared<cProtIntGenZoom>(a_Seed + static_cast<int>(j), gen);
		}
		gen->GetInts(0, 0, arrSize, arrSize, workspace);

		// Output to a bitmap file:
		AString fnam = Printf("zoomedgrown_%u.pbm", i);
		outputBitmapFile(fnam, 257, 257, workspace, arrSize, arrSize, spectrumColors, ARRAYCOUNT(spectrumColors));
		log("  zoom level %u complete", i);
	}  // for i - Image
}





void generateSmoothLevels(int a_Seed)
{
	log("Generating smooth levels...");
	const unsigned NumImages = 7;  ///< Number of images to generate. The more images, the larger they will be
	const unsigned maxArrSize = 65;  ///< Size of the underlying generated array

	// Initialize the underlying generator:
	std::shared_ptr<cProtIntGen> underlyingGen = std::make_shared<cProtIntGenChoice>(a_Seed, static_cast<int>(ARRAYCOUNT(spectrumColors) + 1));
	for (int j = 1; j < 4; j++)
	{
		underlyingGen = std::make_shared<cProtIntGenZoom>(a_Seed + j, underlyingGen);
	}

	// Generate smooth levels:
	for (unsigned i = 1; i <= NumImages; i++)
	{
		unsigned arrSize = maxArrSize - 2 * i;  // Dimensions of the actually generated array
		int workspace[maxArrSize * maxArrSize];  // Workspace for the generated array

		// Chain the zoom operation as many times as the image number:
		std::shared_ptr<cProtIntGen> gen = underlyingGen;
		for (unsigned j = 1; j < i; j++)
		{
			gen = std::make_shared<cProtIntGenSmooth>(a_Seed, gen);
		}
		gen->GetInts(static_cast<int>(i), static_cast<int>(i), arrSize, arrSize, workspace);

		// Output to a bitmap file:
		AString fnam = Printf("smoothedgrown_%u.ppm", i);
		outputBitmapFile(fnam, 257, 257, workspace, arrSize, arrSize, spectrumColors, ARRAYCOUNT(spectrumColors));
		log("  smooth level %u complete", i);
	}  // for i - Image
}





void generateExamples(int a_Seed)
{
	log("Generating examples");

	const int maxArrSize = 65;
	const int inArrSize = 24;
	const int imgSize = 256;

	// Create the inputs:
	auto in1 =
		std::make_shared<cProtIntGenZoom           >(a_Seed + 1,
		std::make_shared<cProtIntGenAddIslands     >(a_Seed + 2000, 200,
		std::make_shared<cProtIntGenSetRandomly    >(a_Seed + 9, 300, bgOcean,
		std::make_shared<cProtIntGenZoom           >(a_Seed + 2,
		std::make_shared<cProtIntGenLandOcean      >(a_Seed + 1, 30
	)))));
	auto in2 =
		std::make_shared<cProtIntGenZoom           >(a_Seed + 1,
		std::make_shared<cProtIntGenBiomeGroupEdges>(in1
	));
	auto in3 =
		std::make_shared<cProtIntGenZoom           >(a_Seed + 1,
		std::make_shared<cProtIntGenZoom           >(a_Seed + 2,
		std::make_shared<cProtIntGenBiomes         >(a_Seed + 3000, in2
	)));
	auto inAlt =
		std::make_shared<cProtIntGenZoom           >(a_Seed,
		std::make_shared<cProtIntGenLandOcean      >(a_Seed, 30
	));
	auto inRiver = std::make_shared<cProtIntGenRiver>(a_Seed, in2);
	int workspace[maxArrSize * maxArrSize];
	in1->GetInts(0, 0, inArrSize, inArrSize, workspace);
	outputBitmapFile("grownexample_in1.ppm", imgSize, imgSize, workspace, inArrSize, inArrSize, biomeGroupColors, ARRAYCOUNT(biomeGroupColors));
	log("  Created example input 1");
	in2->GetInts(0, 0, inArrSize, inArrSize, workspace);
	outputBitmapFile("grownexample_in2.ppm", imgSize, imgSize, workspace, inArrSize, inArrSize, biomeGroupColors, ARRAYCOUNT(biomeGroupColors));
	log("  Created example input 2");
	in3->GetInts(0, 0, inArrSize, inArrSize, workspace);
	outputBitmapFile("grownexample_in3.ppm", imgSize, imgSize, workspace, inArrSize, inArrSize, biomeColors, ARRAYCOUNT(biomeColors));
	log("  Created example input 3");
	inAlt->GetInts(0, 0, inArrSize, inArrSize, workspace);
	outputBitmapFile("grownexample_in_alt.ppm", imgSize, imgSize, workspace, inArrSize, inArrSize, biomeGroupColors, ARRAYCOUNT(biomeGroupColors));
	log("  Created example input alt");
	inRiver->GetInts(0, 0, inArrSize, inArrSize, workspace);
	outputBitmapFile("grownexample_in_river.ppm", imgSize, imgSize, workspace, inArrSize, inArrSize, biomeColors, ARRAYCOUNT(biomeColors));
	log("  Created example input river");

	// Shortcuts for colormaps used for the outputs:
	struct ColorMap
	{
		const Color * colors;
		size_t count;
	};
	static const ColorMap cmGroups = { biomeGroupColors, ARRAYCOUNT(biomeGroupColors) };
	static const ColorMap cmBiomes = { biomeColors,      ARRAYCOUNT(biomeColors) };

	// Create the result generators:
	struct
	{
		const char * name;
		unsigned size;
		int offset;
		const ColorMap & colormap;
		std::shared_ptr<cProtIntGen> gen;
	}
	gens[] =
	{
		{"add_islands", inArrSize,         0, cmGroups, std::make_shared<cProtIntGenAddIslands>     (a_Seed, 400, in1)},
		{"alt_biomes",  inArrSize,         0, cmBiomes, std::make_shared<cProtIntGenAlternateBiomes>(a_Seed, inAlt, in3)},
		{"beaches",     inArrSize - 2,     1, cmBiomes, std::make_shared<cProtIntGenBeaches>        (in3)},
		{"biome_edges", inArrSize - 2,     1, cmBiomes, std::make_shared<cProtIntGenBiomeEdges>     (a_Seed, in3)},
		{"biomes",      inArrSize,         0, cmBiomes, std::make_shared<cProtIntGenBiomes>         (a_Seed, in2)},
		{"grp_edges",   inArrSize - 2,     0, cmGroups, std::make_shared<cProtIntGenBiomeGroupEdges>(in1)},
		{"m_biomes",    inArrSize,         0, cmBiomes, std::make_shared<cProtIntGenMBiomes>        (a_Seed, inAlt, in3)},
		{"mix_river",   inArrSize,         0, cmBiomes, std::make_shared<cProtIntGenMixRivers>      (in3, inRiver)},
		{"river",       inArrSize - 2,     1, cmBiomes, inRiver},
		{"set_rnd",     inArrSize,         0, cmBiomes, std::make_shared<cProtIntGenSetRandomly>    (a_Seed, 500, bgOcean, in3)},
		{"smooth",      inArrSize - 2,     1, cmBiomes, std::make_shared<cProtIntGenSmooth>         (a_Seed, in3)},
		{"zoom",        inArrSize * 2 - 1, 0, cmBiomes, std::make_shared<cProtIntGenZoom>           (a_Seed, in3)},
	};

	// Create the outputs:
	for (const auto & gen: gens)
	{
		gen.gen->GetInts(gen.offset, gen.offset, gen.size, gen.size, workspace);
		AString fnam = Printf("grownexample_%s.ppm", gen.name);
		outputBitmapFile(fnam, 256, 256, workspace, gen.size, gen.size, gen.colormap.colors, gen.colormap.count);
		log("  Created example \"%s\"", gen.name);
	}  // for gen - gens[]

	log("Examples generated");
}





int main(int argc, char ** argv)
{
	log("GrownBiomeGenVisualiser starting");

	// Parse the seed from the command line, if present:
	int seed;
	if (argc > 1)
	{
		if (!StringToInteger(argv[1], seed))
		{
			log("Cannot parse seed from \"%s\", bailing out.", argv[1]);
			return 1;
		}
	}
	else
	{
		// Get a random seed:
		std::random_device rd;
		seed = static_cast<int>(rd());
	}

	log("Seed = %d", seed);

	initializeBiomeColors();

	generateZoomLevels(seed);
	generateSmoothLevels(seed);
	generateExamples(seed);

	log("GrownBiomeGenVisualiser finished");
	return 0;
}
