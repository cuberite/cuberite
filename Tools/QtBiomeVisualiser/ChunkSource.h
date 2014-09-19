#pragma once
#include <QString>
#include <QMutex>
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

	/** The generator used for generating biomes. */
	std::unique_ptr<cBiomeGen> m_BiomeGen;

	/** Guards m_BiomeGen against multithreaded access. */
	QMutex m_Mtx;
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




