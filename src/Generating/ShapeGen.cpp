
// ShapeGen.cpp

// Implements the function to create a cTerrainShapeGen descendant based on INI file settings

#include "Globals.h"
#include "HeiGen.h"
#include "../IniFile.h"
#include "DistortedHeightmap.h"
#include "EndGen.h"
#include "Noise3DGenerator.h"
#include "TwoHeights.h"





////////////////////////////////////////////////////////////////////////////////
// cTerrainHeightToShapeGen:

/** Converts old-style height-generators into new-style shape-generators. */
class cTerrainHeightToShapeGen:
	public cTerrainShapeGen
{
public:
	cTerrainHeightToShapeGen(cTerrainHeightGenPtr a_HeightGen):
		m_HeightGen(std::move(a_HeightGen))
	{
	}


	// cTerrainShapeGen overrides:
	virtual void GenShape(cChunkCoords a_ChunkCoords, cChunkDesc::Shape & a_Shape) override
	{
		// Generate the heightmap:
		cChunkDef::HeightMap heightMap;
		m_HeightGen->GenHeightMap(a_ChunkCoords, heightMap);

		// Convert from heightmap to shape:
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int x = 0; x < cChunkDef::Width; x++)
			{
				int height = cChunkDef::GetHeight(heightMap, x, z) + 1;
				Byte * shapeColumn = &(a_Shape[(x + 16 * z) * 256]);
				for (int y = 0; y < height; y++)
				{
					shapeColumn[y] = 1;
				}
				for (int y = height; y < cChunkDef::Height; y++)
				{
					shapeColumn[y] = 0;
				}
			}  // for x
		}  // for z
	}


	virtual void InitializeShapeGen(cIniFile & a_IniFile) override
	{
		m_HeightGen->InitializeHeightGen(a_IniFile);
	}

protected:
	/** The height generator being converted. */
	cTerrainHeightGenPtr m_HeightGen;
};

typedef std::shared_ptr<cTerrainHeightToShapeGen> cTerrainHeightToShapeGenPtr;





////////////////////////////////////////////////////////////////////////////////
// cTerrainShapeGen:

cTerrainShapeGenPtr cTerrainShapeGen::CreateShapeGen(
	cIniFile & a_IniFile,
	const cBiomeGenPtr & a_BiomeGen,
	int a_Seed,
	bool & a_CacheOffByDefault
)
{
	AString shapeGenName = a_IniFile.GetValue("Generator", "ShapeGen");
	if (shapeGenName.empty())
	{
		LOGWARN("[Generator] ShapeGen value not set in world.ini, using \"BiomalNoise3D\".");
		shapeGenName = "BiomalNoise3D";
	}

	// If the shapegen is HeightMap, redirect to older HeightMap-based generators:
	if (NoCaseCompare(shapeGenName, "HeightMap") == 0)
	{
		cTerrainHeightGenPtr heightGen = cTerrainHeightGen::CreateHeightGen(a_IniFile, a_BiomeGen, a_Seed, a_CacheOffByDefault);
		if (heightGen != nullptr)
		{
			return std::make_shared<cTerrainHeightToShapeGen>(heightGen);
		}

		// The height gen was not recognized; several heightgens were promoted to shape gens, so let's try them instead:
		shapeGenName = a_IniFile.GetValue("Generator", "HeightGen", "");
		if (shapeGenName.empty())
		{
			LOGWARNING("[Generator] ShapeGen set to HeightMap, but HeightGen not set. Reverting to \"BiomalNoise3D\".");
			shapeGenName = "BiomalNoise3D";
			a_IniFile.SetValue("Generator", "ShapeGen", shapeGenName);
		}
	}

	// Choose the shape generator based on the name:
	a_CacheOffByDefault = false;
	cTerrainShapeGenPtr res;
	if (NoCaseCompare(shapeGenName, "DistortedHeightmap") == 0)
	{
		res = std::make_shared<cDistortedHeightmap>(a_Seed, a_BiomeGen);
	}
	else if (NoCaseCompare(shapeGenName, "End") == 0)
	{
		res = std::make_shared<cEndGen>(a_Seed);
	}
	else if (NoCaseCompare(shapeGenName, "BiomalNoise3D") == 0)
	{
		res = std::make_shared<cBiomalNoise3DComposable>(a_Seed, a_BiomeGen);
	}
	else if (NoCaseCompare(shapeGenName, "Noise3D") == 0)
	{
		res = std::make_shared<cNoise3DComposable>(a_Seed);
	}
	else if (NoCaseCompare(shapeGenName, "TwoHeights") == 0)
	{
		res = CreateShapeGenTwoHeights(a_Seed, a_BiomeGen);
	}
	else
	{
		// No match found, force-set the default and retry
		LOGWARN("Unknown ShapeGen \"%s\", using \"BiomalNoise3D\" instead.", shapeGenName.c_str());
		a_IniFile.SetValue("Generator", "ShapeGen", "BiomalNoise3D");
		return CreateShapeGen(a_IniFile, a_BiomeGen, a_Seed, a_CacheOffByDefault);
	}

	// Read the settings:
	res->InitializeShapeGen(a_IniFile);

	return res;
}




