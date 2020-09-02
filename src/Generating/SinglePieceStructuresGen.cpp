
#include "SinglePieceStructuresGen.h"

#include "PrefabStructure.h"
#include "../IniFile.h"
#include "../Item.h"


////////////////////////////////////////////////////////////////////////////////
// cSinglePieceStructuresGen::cGen

class cSinglePieceStructuresGen::cGen :
	public cGridStructGen
{
	using Super = cGridStructGen;
public:
	cGen(int a_Seed, cBiomeGenPtr a_BiomeGen, cTerrainHeightGenPtr a_HeightGen, int a_SeaLevel, const AString & a_Name):
		Super(a_Seed),
		m_BiomeGen(std::move(a_BiomeGen)),
		m_HeightGen(std::move(a_HeightGen)),
		m_SeaLevel(a_SeaLevel),
		m_Name(a_Name)
	{
	}



	/** Loads the piecepool from a file.
	Returns true on success, logs warning and returns false on failure. */
	bool LoadFromFile(const AString & a_FileName)
	{
		m_PiecePool.Clear();

		// Load the piecepool from the file, log any warnings:
		if (!m_PiecePool.LoadFromFile(a_FileName, true))
		{
			return false;
		}
		if (NoCaseCompare(m_PiecePool.GetIntendedUse(), "SinglePieceStructures") != 0)
		{
			LOGWARNING("SinglePieceStructures generator: File %s is intended for use in \"%s\", rather than single piece structures. Loading the file, but the generator may behave unexpectedly.",
				a_FileName.c_str(), m_PiecePool.GetIntendedUse().c_str()
			);
		}
		m_PiecePool.AssignGens(m_Seed, m_BiomeGen, m_HeightGen, m_SeaLevel);

		// Apply generator params from the piecepool (in the metadata) into the generator:
		auto & generatorParams = m_PiecePool.GetAllMetadata();
		SetGeneratorParams(generatorParams);

		return true;
	}




	// cGridStructGen override
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override
	{
		// Generate the biomes for the chunk surrounding the origin:
		int ChunkX, ChunkZ;
		cChunkDef::BlockToChunk(a_OriginX, a_OriginZ, ChunkX, ChunkZ);
		cChunkDef::BiomeMap Biomes;
		m_BiomeGen->GenBiomes({ChunkX, ChunkZ}, Biomes);

		// Checks if the biome at the origin position is allowed
		if (!m_PiecePool.IsBiomeAllowed(Biomes[ChunkX + cChunkDef::Width * ChunkZ]))
		{
			return cStructurePtr();
		}
		cPlacedPieces OutPiece;
		OutPiece.push_back(GetPiece(a_OriginX, a_OriginZ));
		return std::make_shared<cPrefabStructure>(a_GridX, a_GridZ, a_OriginX, a_OriginZ, std::move(OutPiece), m_HeightGen);
	}




	/** Determines which piece to place from the piece pool */
	cPlacedPiecePtr GetPiece(int a_BlockX, int a_BlockZ)
	{
		int rnd = m_Noise.IntNoise2DInt(a_BlockX, a_BlockZ) / 7;

		// Choose a random one of the starting pieces:
		cPieces StartingPieces = m_PiecePool.GetStartingPieces();
		int Total = 0;
		for (cPieces::const_iterator itr = StartingPieces.begin(), end = StartingPieces.end(); itr != end; ++itr)
		{
			Total += m_PiecePool.GetStartingPieceWeight(**itr);
		}
		cPiece * StartingPiece;
		if (Total > 0)
		{
			int Chosen = rnd % Total;
			StartingPiece = StartingPieces.front();
			for (cPieces::const_iterator itr = StartingPieces.begin(), end = StartingPieces.end(); itr != end; ++itr)
			{
				Chosen -= m_PiecePool.GetStartingPieceWeight(**itr);
				if (Chosen <= 0)
				{
					StartingPiece = *itr;
					break;
				}
			}
		}
		else
		{
			// All pieces returned zero weight, but we need one to start. Choose with equal chance:
			StartingPiece = StartingPieces[static_cast<size_t>(rnd) % StartingPieces.size()];
		}
		rnd = rnd >> 16;

		// Choose a random supported rotation:
		int Rotations[4] = {0};
		int NumRotations = 1;
		for (size_t i = 1; i < ARRAYCOUNT(Rotations); i++)
		{
			if (StartingPiece->CanRotateCCW(static_cast<int>(i)))
			{
				Rotations[NumRotations] = static_cast<int>(i);
				NumRotations += 1;
			}
		}
		int Rotation = Rotations[rnd % NumRotations];
		int BlockY = StartingPiece->GetStartingPieceHeight(a_BlockX, a_BlockZ);
		ASSERT(BlockY >= 0);  // The vertical strategy should have been provided and should give valid coords

		cPlacedPiece * Piece = new cPlacedPiece(nullptr, *StartingPiece, Vector3i(a_BlockX, BlockY, a_BlockZ), Rotation);
		return cPlacedPiecePtr(Piece);
	}

protected:
	/** The underlying biome generator that defines whether the structure is created or not */
	cBiomeGenPtr m_BiomeGen;

	/** The underlying height generator, used to position the prefabs crossing chunk borders if they are set to FitGround. */
	cTerrainHeightGenPtr m_HeightGen;

	/** The world's sea level, if available. Used for some cVerticalStrategy descendants. */
	int m_SeaLevel;

	/** The name that is used for reporting. */
	AString m_Name;

	/** All available prefabs. */
	cPrefabPiecePool m_PiecePool;
};


////////////////////////////////////////////////////////////////////////////////
// cSinglePieceStructuresGen

cSinglePieceStructuresGen::cSinglePieceStructuresGen(int a_Seed) :
	m_Seed(a_Seed)
{
}





bool cSinglePieceStructuresGen::Initialize(const AString & a_Prefabs, int a_SeaLevel, const cBiomeGenPtr & a_BiomeGen, const cTerrainHeightGenPtr & a_HeightGen)
{
	// Load each piecepool:
	auto Structures = StringSplitAndTrim(a_Prefabs, "|");
	for (const auto & S: Structures)
	{
		auto FileName = Printf("Prefabs%cSinglePieceStructures%c%s.cubeset", cFile::PathSeparator(), cFile::PathSeparator(), S.c_str());
		if (!cFile::IsFile(FileName))
		{
			FileName.append(".gz");
			if (!cFile::IsFile(FileName))
			{
				LOGWARNING("Cannot load SinglePieceStructure cubeset file %s", FileName.c_str());
				continue;
			}
		}

		auto Gen = std::make_shared<cGen>(m_Seed, a_BiomeGen, a_HeightGen, a_SeaLevel, S);
		if (Gen->LoadFromFile(FileName))
		{
			m_Gens.push_back(Gen);
		}
	}

	// Report a warning if no generators available:
	if (m_Gens.empty())
	{
		LOGWARNING("The PieceStructures generator was asked to generate \"%s\", but none of the prefabs are valid.", a_Prefabs.c_str());
		return false;
	}
	return true;
}





void cSinglePieceStructuresGen::GenFinish(cChunkDesc & a_Chunk)
{
	for (auto & Gen: m_Gens)
	{
		Gen->GenFinish(a_Chunk);
	}
}
