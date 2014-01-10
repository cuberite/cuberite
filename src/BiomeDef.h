
// BiomeDef.h

// Defines relevant information and methods related to biomes





#pragma once





// tolua_begin
/** Biome IDs
The first batch corresponds to the clientside biomes, used by MineCraft.
BiomeIDs over 255 are used by MCServer internally and are translated to MC biomes before sending them to client
*/
enum EMCSBiome
{
	biOcean            = 0,
	biPlains           = 1,
	biDesert           = 2,
	biExtremeHills     = 3,
	biForest           = 4,
	biTaiga            = 5,
	biSwampland        = 6,
	biRiver            = 7,
	biHell             = 8,  // same as Nether
	biNether           = 8,
	biSky              = 9,  // same as biEnd
	biEnd              = 9,
	biFrozenOcean      = 10,
	biFrozenRiver      = 11,
	biIcePlains        = 12,
	biTundra           = 12,  // same as Ice Plains
	biIceMountains     = 13,
	biMushroomIsland   = 14,
	biMushroomShore    = 15,
	biBeach            = 16,
	biDesertHills      = 17,
	biForestHills      = 18,
	biTaigaHills       = 19,
	biExtremeHillsEdge = 20,
	biJungle           = 21,
	biJungleHills      = 22,
	
	// Release 1.7 biomes:
	biJungleEdge       = 23,
	biDeepOcean        = 24,
	biStoneBeach       = 25,
	biColdBeach        = 26,
	biBirchForest      = 27,
	biBirchForestHills = 28,
	biRoofedForest     = 29,
	biColdTaiga        = 30,
	biColdTaigaHills   = 31,
	biMegaTaiga        = 32,
	biMegaTaigaHills   = 33,
	biExtremeHillsPlus = 34,
	biSavanna          = 35,
	biSavannaPlateau   = 36,
	biMesa             = 37,
	biMesaPlateauF     = 38,
	biMesaPlateau      = 39,
	
	// Automatically capture the maximum consecutive biome value into biMaxBiome:
	biNumBiomes,  // True number of biomes, since they are zero-based
	biMaxBiome = biNumBiomes - 1,  // The maximum biome value
	
	// Add this number to the biomes to get the variant
	biVariant = 128,
	
	// Release 1.7 biome variants:
	biSunflowerPlains      = 129,
	biDesertM              = 130,
	biExtremeHillsM        = 131,
	biFlowerForest         = 132,
	biTaigaM               = 133,
	biSwamplandM           = 134,
	biIcePlainsSpikes      = 140,
	biJungleM              = 149,
	biJungleEdgeM          = 151,
	biBirchForestM         = 155,
	biBirchForestHillsM    = 156,
	biRoofedForestM        = 157,
	biColdTaigaM           = 158,
	biMegaSpruceTaiga      = 160,
	biMegaSpruceTaigaHills = 161,
	biExtremeHillsPlusM    = 162,
	biSavannaM             = 163,
	biSavannaPlateauM      = 164,
	biMesaBryce            = 165,
	biMesaPlateauFM        = 166,
	biMesaPlateauM         = 167,
} ;

/// Translates a biome string to biome enum. Takes either a number or a biome alias (built-in). Returns -1 on failure.
extern EMCSBiome StringToBiome(const AString & a_BiomeString);

/// Returns true if the biome has no downfall - deserts and savannas
extern bool IsBiomeNoDownfall(EMCSBiome a_Biome);


// tolua_end
