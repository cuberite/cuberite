
// PieceStructuresGen.cpp

// Declares the cPieceStructuresGen class representing the PieceStructures finisher generator

#include "Globals.h"
#include "PieceStructuresGen.h"
#include "PrefabStructure.h"
#include "PieceGeneratorBFSTree.h"
#include "../IniFile.h"





class cPieceStructuresGen::cGen:
	public cGridStructGen
{
	using Super = cGridStructGen;

public:

	cGen(int a_Seed, cBiomeGenPtr a_BiomeGen, cTerrainHeightGenPtr a_HeightGen, int a_SeaLevel, const AString & a_Name):
		Super(a_Seed),
		m_BiomeGen(std::move(a_BiomeGen)),
		m_HeightGen(std::move(a_HeightGen)),
		m_SeaLevel(a_SeaLevel),
		m_Name(a_Name),
		m_MaxDepth(5)
	{
	}



	/** Loads the piecepool from a file.
	Returns true on success, logs warning and returns false on failure. */
	bool LoadFromFile(const AString & a_FileName)
	{
		// Load the piecepool from the file, log any warnings:
		if (!m_PiecePool.LoadFromFile(a_FileName, true))
		{
			return false;
		}
		if (NoCaseCompare(m_PiecePool.GetIntendedUse(), "PieceStructures") != 0)
		{
			LOGWARNING("PieceStructures generator: File %s is intended for use in \"%s\", rather than piece structures. Loading the file, but the generator may behave unexpectedly.",
				a_FileName.c_str(), m_PiecePool.GetIntendedUse().c_str()
			);
		}
		m_PiecePool.AssignGens(m_Seed, m_BiomeGen, m_HeightGen, m_SeaLevel);

		// Apply generator params from the piecepool (in the metadata) into the generator:
		auto & GeneratorParams = m_PiecePool.GetAllMetadata();
		SetGeneratorParams(GeneratorParams);
		m_MaxDepth = GetStringMapInteger<int>(GeneratorParams, "MaxDepth", m_MaxDepth);

		return true;
	}



	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override
	{
		cPlacedPieces OutPieces;
		cPieceGeneratorBFSTree PieceTree(m_PiecePool, m_Seed);
		PieceTree.PlacePieces(a_OriginX, a_OriginZ, m_MaxDepth, OutPieces);
		return std::make_shared<cPrefabStructure>(a_GridX, a_GridZ, a_OriginX, a_OriginZ, std::move(OutPieces), m_HeightGen);
	}


protected:

	/** The type used for storing a connection from one piece to another, while building the piece tree. */
	struct cConnection
	{
		cPiece * m_Piece;                  // The piece being connected
		cPiece::cConnector m_Connector;    // The piece's connector being used (relative non-rotated coords)
		int m_NumCCWRotations;             // Number of rotations necessary to match the two connectors
		int m_Weight;                      // Relative chance that this connection will be chosen

		cConnection(cPiece & a_Piece, cPiece::cConnector & a_Connector, int a_NumCCWRotations, int a_Weight);
	};
	typedef std::vector<cConnection> cConnections;


	/** The type used for storing a pool of connectors that will be attempted to expand by another piece. */
	struct cFreeConnector
	{
		cPlacedPiece * m_Piece;
		cPiece::cConnector m_Connector;

		cFreeConnector(cPlacedPiece * a_Piece, const cPiece::cConnector & a_Connector);
	};
	typedef std::vector<cFreeConnector> cFreeConnectors;

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

	/** Maximum depth of the generated piece tree. */
	int m_MaxDepth;
};





////////////////////////////////////////////////////////////////////////////////
// cPieceStructuresGen:

cPieceStructuresGen::cPieceStructuresGen(int a_Seed):
	m_Seed(a_Seed)
{
}





bool cPieceStructuresGen::Initialize(const AString & a_Prefabs, int a_SeaLevel, const cBiomeGenPtr & a_BiomeGen, const cTerrainHeightGenPtr & a_HeightGen)
{
	// Load each piecepool:
	auto Structures = StringSplitAndTrim(a_Prefabs, "|");
	for (const auto & Structure : Structures)
	{
		auto FileName = Printf("Prefabs%cPieceStructures%c%s.cubeset", cFile::PathSeparator(), cFile::PathSeparator(), Structure.c_str());
		if (!cFile::IsFile(FileName))
		{
			FileName.append(".gz");
			if (!cFile::IsFile(FileName))
			{
				LOGWARNING("Cannot load PieceStructures cubeset file %s", FileName.c_str());
				continue;
			}
		}
		auto Gen = std::make_shared<cGen>(m_Seed, a_BiomeGen, a_HeightGen, a_SeaLevel, Structure);
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





void cPieceStructuresGen::GenFinish(cChunkDesc & a_Chunk)
{
	for (auto & Gen : m_Gens)
	{
		Gen->GenFinish(a_Chunk);
	}
}





