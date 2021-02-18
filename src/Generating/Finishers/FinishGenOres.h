
// FinishGenOres.h

#pragma once

#include "../ComposableGenerator.h"
#include "../../Noise/Noise.h"
#include "../../IniFile.h"

/** Base class for generators that have an ore list attached to them.
Provides the storage and parsing for the ore list, as well as the generic plumbing for generating individual ores.
Descendants should override GenerateOre() to provide the specific ore generation technique.
Note that this class uses the "Nest" terminology for individual packs of ore, it doesn't imply any shape or algorithm. */
class cFinishGenOres:
	public cFinishGen
{
	using Super = cFinishGen;

public:

	struct OreInfo
	{
		BLOCKTYPE  m_BlockType;  // The type of the nest.
		NIBBLETYPE m_BlockMeta;  // The block meta
		int        m_MaxHeight;  // The highest possible a nest can occur
		int        m_NumNests;   // How many nests per chunk
		int        m_NestSize;   // The amount of blocks a nest can have.

		OreInfo() :
			m_BlockType(0),
			m_BlockMeta(0),
			m_MaxHeight(0),
			m_NumNests(0),
			m_NestSize(0)
		{
		}

		OreInfo(BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta, int a_MaxHeight, int a_NumNests, int a_NestSize) :
			m_BlockType(a_OreType),
			m_BlockMeta(a_OreMeta),
			m_MaxHeight(a_MaxHeight),
			m_NumNests(a_NumNests),
			m_NestSize(a_NestSize)
		{
		}
	};

	typedef std::vector<OreInfo> OreInfos;

	cFinishGenOres(int a_Seed, const OreInfos & a_OreInfos):
		m_Noise(a_Seed),
		m_OreInfos(a_OreInfos)
	{
	}

	// cFinishGen overrides:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;

	/** Returns a vector of OreInfo structures describing the default Overworld ores, usable in the constructor. */
	static const OreInfos & DefaultOverworldOres(void);

	/** Returns a vector of OreInfo structures describing the default Nether ores, usable in the constructor. */
	static const OreInfos & DefaultNetherOres(void);

	/** Returns a vector of OreInfo structures describing the default Overworld non-ore pockets (dirt, diorite etc), usable in the constructor. */
	static const OreInfos & DefaultNaturalPatches(void);

	/** Parses the parameter string into OreInfos array.
	See OreInfosToString() for the complementary function.
	Used for loading configuration from INI files. */
	static OreInfos OreInfosFromString(const AString & a_OreInfosString);

	/** Returns a string that represents the OreInfos given as the parameter.
	See OreInfosFromString() for the complementary function.
	Used for storing defaults in the INI file. */
	static AString OreInfosToString(const OreInfos & a_OreInfos);

	/** (Re-)sets the seed used by the internal generating mechanisms. */
	void SetSeed(int a_Seed);

  protected:
	/** The noise used for generating. */
	cNoise m_Noise;

	/** All the ores enabled in this generator. */
	OreInfos m_OreInfos;


	/** Generates a single ore in the specified chunk image.
	a_Seq is the sequencing number (used as a complement to seed to make each ore in the same chunk have different nests) */
	virtual void GenerateOre(
		cChunkDesc & a_ChunkDesc,
		BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta,
		int a_MaxHeight, int a_NumNests, int a_NestSize,
		int a_Seq
	) = 0;

	// TODO: Helper function to parse a config string into m_OreInfos
};





class cFinishGenOreNests:
	public cFinishGenOres
{
	using Super = cFinishGenOres;

  public:

	cFinishGenOreNests(int a_Seed, const OreInfos & a_OreInfos):
		Super(a_Seed, a_OreInfos)
	{}

  protected:

	// cFinishGenOreClumps overrides:
	virtual void GenerateOre(
		cChunkDesc & a_ChunkDesc,
		BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta,
		int a_MaxHeight, int a_NumNests, int a_NestSize,
		int a_Seq
	) override;
} ;





class cFinishGenOrePockets:
	public cFinishGenOres
{
	using Super = cFinishGenOres;

  public:

	cFinishGenOrePockets(int a_Seed, const OreInfos & a_OreInfos):
		Super(a_Seed, a_OreInfos)
	{}

	/** Reads the configuration from the specified INI file.
	a_GenName is the name of the generator (this class may be used for OrePockets and DirtPockets, each has a different default). */
	void Initialize(cIniFile & a_IniFile, const AString & a_GenName);

  protected:

	// cFinishGenOreClumps overrides:
	virtual void GenerateOre(
		cChunkDesc & a_ChunkDesc,
		BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta,
		int a_MaxNestHeight, int a_NumNests, int a_NestSize,
		int a_Seq
	) override;

	/** Calculates the pockets for the specified chunk and imprints them into the specified ChunkDesc (not necessarily the same chunk).
	a_Seq is the sequence number of the ore, to provide another source of randomness. */
	void imprintChunkOrePockets(
		int a_ChunkX, int a_ChunkZ,
		cChunkDesc & a_ChunkDesc,
		BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta,
		int a_MaxHeight, int a_NumNests, int a_NestSize,
		int a_Seq
	);

	/** Imprints a single pocket of the specified ore at the specified coords into the chunk.
	The pocket shape has its minimum X and Z coords specified, Y can be anywhere around the specified Y coord.
	a_Seq is the sequence number of the ore, to provide another source of randomness. */
	void imprintPocket(
		cChunkDesc & a_ChunkDesc,
		int a_MinPocketX, int a_PocketY, int a_MinPocketZ,
		int a_NestSize, int a_Seq,
		BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta
	);

	/** Imprints a single sphere of the specified ore at the specified coords. */
	void imprintSphere(
		cChunkDesc & a_ChunkDesc,
		double a_SphereX, double a_SphereY, double a_SphereZ, double a_Radius,
		BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta
	);
};
