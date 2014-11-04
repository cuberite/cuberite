#pragma once
#include "Globals.h"
#include <QString>
#include <QMutex>
#include "QtChunk.h"





// fwd:
class cBiomeGen;
typedef SharedPtr<cBiomeGen> cBiomeGenPtr;
class cIniFile;
typedef std::shared_ptr<cIniFile> cIniFilePtr;





/** Abstract interface for getting biome data for chunks. */
class ChunkSource
{
public:
	virtual ~ChunkSource() {}

	/** Fills the a_DestChunk with the biomes for the specified coords.
	It is expected to be thread-safe and re-entrant. Usually QThread::idealThreadCount() threads are used. */
	virtual void getChunkBiomes(int a_ChunkX, int a_ChunkZ, Chunk & a_DestChunk) = 0;

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
	virtual void getChunkBiomes(int a_ChunkX, int a_ChunkZ, Chunk & a_DestChunk) override;
	virtual void reload(void) override;

protected:
	/** The world.ini contents from which the generator is created and re-created on reload(). */
	cIniFilePtr m_IniFile;

	/** The generator used for generating biomes. */
	std::vector<cBiomeGenPtr> m_BiomeGens;

	/** Guards m_BiomeGens against multithreaded access. */
	QMutex m_Mtx;

	/** Keeps track of the current settings of the biomegens.
	Incremented by one each time reload() is called. Provides the means of releasing old biomegens that were
	in use while reload() was being processed and thus couldn't be changed back then. releaseBiomeGen() does
	the job of filtering the biogens before reusing them. */
	int m_CurrentTag;


	/** Retrieves one cBiomeGenPtr from m_BiomeGens.
	If there's no biogen available there, creates a new one based on the ini file.
	When done with it, the caller should call releaseBiomeGen() to put the biogen back to m_BiomeGens.
	a_Tag receives the value of m_CurrentTag from when the lock was held; it should be passed to
	releaseBiomeGen() together with the biogen. */
	cBiomeGenPtr getBiomeGen(int & a_Tag);

	/** Marks the specified biogen as available for reuse (puts it back into m_BiomeGens).
	a_Tag is the value of m_CurrentTag from the time when the biogen was retrieved; if it is different from
	current m_CurrentTagValue, the biogen will be disposed of (because reload() has been called in the
	meantime). */
	void releaseBiomeGen(cBiomeGenPtr && a_BiomeGen, int a_Tag);
};




class AnvilSource :
	public ChunkSource
{
public:
	/** Constructs a new AnvilSource based on the world path. */
	AnvilSource(QString a_WorldRegionFolder);

	// ChunkSource overrides:
	virtual void getChunkBiomes(int a_ChunkX, int a_ChunkZ, Chunk & a_DestChunk) override;
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




