#pragma once
#include <QString>
#include "Chunk.h"





// fwd:
class cBiomeGen;
typedef std::shared_ptr<cBiomeGen> cBiomeGenPtr;
class cIniFile;





/** Abstract interface for getting biome data for chunks. */
class ChunkSource
{
public:
	virtual ~ChunkSource() {}

	/** Fills the a_DestChunk with the biomes for the specified coords.
	It is expected to be thread-safe and re-entrant. Usually QThread::idealThreadCount() threads are used. */
	virtual void getChunkBiomes(int a_ChunkX, int a_ChunkZ, ChunkPtr a_DestChunk) = 0;

	/** Forces a fresh reload of the source. Useful mainly for the generator, whose underlying definition file may have been changed. */
	virtual void reload() = 0;
};






class BioGenSource :
	public ChunkSource
{
public:
	/** Constructs a new BioGenSource based on the biome generator that is defined in the specified world.ini file. */
	BioGenSource(QString a_WorldIniPath);

	// ChunkSource overrides:
	virtual void getChunkBiomes(int a_ChunkX, int a_ChunkZ, ChunkPtr a_DestChunk) override;
	virtual void reload(void) override;

protected:
	/** Path to the world.ini file from which the m_WorldIni is regenerated on reload requests. */
	QString m_WorldIniPath;

	/** Parsed contents of the world.ini file from which the biome generators are initialized.
	Locked by m_Mtx to avoid multithreaded access. */
	std::unique_ptr<cIniFile> m_WorldIni;

	/** List of cBiomeGen instances that are "free" - aren't doing any generating at this moment.
	Locked by m_Mtx to avoid multithreaded access. */
	std::vector<cBiomeGenPtr> m_AvailableGens;

	/** Guards m_AvailableGens and m_WorldIni against multithreaded access. */
	QMutex m_Mtx;


	/** Returns a cBiomeGen that can generate a new chunk's biomes.
	Uses m_AvailableGens as a cache before creating a new generator. */
	cBiomeGenPtr BioGenSource::getBiomeGen();

	/** Puts the specified BiomeGen back to m_AvailableGens to make it available for next getBiomeGen() request.
	Truncates m_AvailableGens if there are too many instances in there. */
	void releaseBiomeGen(cBiomeGenPtr a_BiomeGen);
};




class AnvilSource :
	public ChunkSource
{
public:
	// TODO

	// ChunkSource overrides:
	virtual void getChunkBiomes(int a_ChunkX, int a_ChunkZ, ChunkPtr a_DestChunk) override;
	virtual void reload() override {}
};




