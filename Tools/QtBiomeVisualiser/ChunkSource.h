#pragma once
#include "Globals.h"
#include <QString>
#include <QMutex>
#include "Chunk.h"





// fwd:
class cBiomeGen;
typedef std::shared_ptr<cBiomeGen> cBiomeGenPtr;
class cIniFile;
typedef std::shared_ptr<cIniFile> cIniFilePtr;





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
	BioGenSource(cIniFilePtr a_IniFile);

	// ChunkSource overrides:
	virtual void getChunkBiomes(int a_ChunkX, int a_ChunkZ, ChunkPtr a_DestChunk) override;
	virtual void reload(void) override;

protected:
	/** The world.ini contents from which the generator is created and re-created on reload(). */
	cIniFilePtr m_IniFile;

	/** The generator used for generating biomes. */
	std::unique_ptr<cBiomeGen> m_BiomeGen;

	/** Guards m_BiomeGen against multithreaded access. */
	QMutex m_Mtx;
};




class AnvilSource :
	public ChunkSource
{
public:
	/** Constructs a new AnvilSource based on the world path. */
	AnvilSource(QString a_WorldRegionFolder);

	// ChunkSource overrides:
	virtual void getChunkBiomes(int a_ChunkX, int a_ChunkZ, ChunkPtr a_DestChunk) override;
	virtual void reload() override;

protected:
	class AnvilFile;
	typedef std::shared_ptr<AnvilFile> AnvilFilePtr;


	/** Folder where the individual Anvil Region files are located. */
	QString m_WorldRegionFolder;

	/** List of currently loaded files. Acts as a cache so that a file is not opened and closed over and over again.
	Protected against multithreaded access by m_Mtx. */
	std::list<AnvilFilePtr> m_Files;

	/** Guards m_Files agains multithreaded access. */
	QMutex m_Mtx;


	/** Converts chunk coords to region coords. */
	void chunkToRegion(int a_ChunkX, int a_ChunkZ, int & a_RegionX, int & a_RegionZ);

	/** Returns the compressed data of the specified chunk.
	Returns an empty string if the chunk is not available. */
	AString getCompressedChunkData(int a_ChunkX, int a_ChunkZ);

	/** Returns the file object that contains the specified chunk.
	The file is taken from the cache if available there, otherwise it is created anew. */
	AnvilFilePtr getAnvilFile(int a_ChunkX, int a_ChunkZ);

};




