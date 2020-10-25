
// TwoHeights.cpp

// Implements the cTwoHeights class representing the terrain shape generator using two switched heightmaps

#include "Globals.h"
#include "TwoHeights.h"
#include "../Noise/InterpolNoise.h"
#include "HeiGen.h"
#include "../LinearUpscale.h"
#include "../IniFile.h"





class cTwoHeights:
	public cTerrainShapeGen
{
	using Super = cTerrainShapeGen;

public:

	cTwoHeights(int a_Seed, const cBiomeGenPtr & a_BiomeGen):
		m_Seed(a_Seed),
		m_Choice(a_Seed),
		m_HeightA(a_Seed + 1, a_BiomeGen),
		m_HeightB(a_Seed + 2, a_BiomeGen)
	{
	}


	// cTerrainShapeGen override:
	virtual void GenShape(cChunkCoords a_ChunkCoords, cChunkDesc::Shape & a_Shape) override
	{
		// Generate the two heightmaps:
		cChunkDef::HeightMap heightsA;
		cChunkDef::HeightMap heightsB;
		m_HeightA.GenHeightMap(a_ChunkCoords, heightsA);
		m_HeightB.GenHeightMap(a_ChunkCoords, heightsB);

		// Generate the choice noise:
		NOISE_DATATYPE smallChoice[33 * 5 * 5];
		NOISE_DATATYPE workspace[33 * 5 * 5];
		NOISE_DATATYPE startX = 0;
		NOISE_DATATYPE endX = 256 * m_FrequencyY;
		NOISE_DATATYPE startY =  a_ChunkCoords.m_ChunkX * cChunkDef::Width * m_FrequencyX;
		NOISE_DATATYPE endY   = (a_ChunkCoords.m_ChunkX * cChunkDef::Width + cChunkDef::Width + 1) * m_FrequencyX;
		NOISE_DATATYPE startZ =  a_ChunkCoords.m_ChunkZ * cChunkDef::Width * m_FrequencyZ;
		NOISE_DATATYPE endZ   = (a_ChunkCoords.m_ChunkZ * cChunkDef::Width + cChunkDef::Width + 1) * m_FrequencyZ;
		m_Choice.Generate3D(smallChoice, 33, 5, 5, startX, endX, startY, endY, startZ, endZ, workspace);
		NOISE_DATATYPE choice[257 * 17 * 17];
		LinearUpscale3DArray(smallChoice, 33, 5, 5, choice, 8, 4, 4);

		// Generate the shape:
		int idxShape = 0;
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int x = 0; x < cChunkDef::Width; x++)
			{
				int idxChoice = 257 * 17 * z + 257 * x;
				NOISE_DATATYPE heightA = static_cast<NOISE_DATATYPE>(cChunkDef::GetHeight(heightsA, x, z));
				NOISE_DATATYPE heightB = static_cast<NOISE_DATATYPE>(cChunkDef::GetHeight(heightsB, x, z));
				for (int y = 0; y < cChunkDef::Height; y++)
				{
					int height = static_cast<int>(ClampedLerp(heightA, heightB, choice[idxChoice++]));
					a_Shape[idxShape++] = (y < height) ? 1 : 0;
				}
			}  // for x
		}  // for z
	}


	virtual void InitializeShapeGen(cIniFile & a_IniFile) override
	{
		m_FrequencyX = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "TwoHeightsFrequencyX", 40));
		m_FrequencyY = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "TwoHeightsFrequencyY", 40));
		m_FrequencyZ = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "TwoHeightsFrequencyZ", 40));

		// Initialize the two underlying height generators from an empty INI file:
		cIniFile empty;
		m_HeightA.InitializeHeightGen(empty);
		m_HeightB.InitializeHeightGen(empty);

		// Add the choice octaves:
		NOISE_DATATYPE freq = 0.001f;
		NOISE_DATATYPE ampl = 1;
		for (int i = 0; i < 4; i++)
		{
			m_Choice.AddOctave(freq, ampl);
			freq = freq * 2;
			ampl = ampl / 2;
		}
	}

protected:
	int m_Seed;

	/** The noise used to decide between the two heightmaps. */
	cOctavedNoise<cInterpolNoise<Interp5Deg>> m_Choice;

	/** The first height generator. */
	cHeiGenBiomal m_HeightA;

	/** The second height generator. */
	cHeiGenBiomal m_HeightB;

	/** The base frequencies for m_Choice in each of the world axis directions. */
	NOISE_DATATYPE m_FrequencyX, m_FrequencyY, m_FrequencyZ;
};





cTerrainShapeGenPtr CreateShapeGenTwoHeights(int a_Seed, const cBiomeGenPtr & a_BiomeGen)
{
	return std::make_shared<cTwoHeights>(a_Seed, a_BiomeGen);
}




