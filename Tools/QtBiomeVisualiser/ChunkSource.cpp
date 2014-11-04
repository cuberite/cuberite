#include "Globals.h"
#include "ChunkSource.h"
#include <QThread>
#include "src/Generating/BioGen.h"
#include "src/StringCompression.h"
#include "src/WorldStorage/FastNBT.h"
#include "src/IniFile.h"





////////////////////////////////////////////////////////////////////////////////
// BioGenSource:

BioGenSource::BioGenSource(cIniFilePtr a_IniFile) :
	m_IniFile(a_IniFile),
	m_Mtx(QMutex::Recursive)
{
	reload();
}





void BioGenSource::getChunkBiomes(int a_ChunkX, int a_ChunkZ, Chunk & a_DestChunk)
{
	cChunkDef::BiomeMap biomes;
	int tag;
	cBiomeGenPtr biomeGen = getBiomeGen(tag);
	biomeGen->GenBiomes(a_ChunkX, a_ChunkZ, biomes);
	releaseBiomeGen(std::move(biomeGen), tag);
	a_DestChunk.setBiomes(biomes);
}





void BioGenSource::reload()
{
	QMutexLocker lock(&m_Mtx);
	m_CurrentTag += 1;
	m_BiomeGens.clear();
}





cBiomeGenPtr BioGenSource::getBiomeGen(int & a_Tag)
{
	QMutexLocker lock(&m_Mtx);
	a_Tag = m_CurrentTag;
	if (m_BiomeGens.empty())
	{
		// Create a new biogen:
		lock.unlock();
		int seed = m_IniFile->GetValueSetI("Seed", "Seed", 0);
		bool unused;
		cBiomeGenPtr res = cBiomeGen::CreateBiomeGen(*m_IniFile, seed, unused);
		return res;
	}
	else
	{
		// Return an existing biogen:
		cBiomeGenPtr res = m_BiomeGens.back();
		m_BiomeGens.pop_back();
		return res;
	}
}





void BioGenSource::releaseBiomeGen(cBiomeGenPtr && a_BiomeGen, int a_Tag)
{
	QMutexLocker lock(&m_Mtx);

	// If the tag differs, the source has been reloaded and this biogen is old, dispose:
	if (a_Tag != m_CurrentTag)
	{
		return;
	}

	// The tag is the same, put the biogen back to list:
	m_BiomeGens.push_back(std::move(a_BiomeGen));
}





////////////////////////////////////////////////////////////////////////////////
// AnvilSource::AnvilFile

class AnvilSource::AnvilFile
{
public:
	/** Coordinates of the region file. */
	int m_RegionX, m_RegionZ;

	/** True iff the file contains proper data. */
	bool m_IsValid;



	/** Creates a new instance with the specified region coords. Reads the file header. */
	AnvilFile(int a_RegionX, int a_RegionZ, const AString & a_WorldPath) :
		m_RegionX(a_RegionX),
		m_RegionZ(a_RegionZ),
		m_IsValid(false)
	{
		readFile(Printf("%s/r.%d.%d.mca", a_WorldPath.c_str(), a_RegionX, a_RegionZ));
	}



	/** Returns the compressed data of the specified chunk.
	Returns an empty string when chunk not present. */
	AString getChunkData(int a_ChunkX, int a_ChunkZ)
	{
		if (!m_IsValid)
		{
			return "";
		}

		// Translate to local coords:
		int RelChunkX = a_ChunkX - m_RegionX * 32;
		int RelChunkZ = a_ChunkZ - m_RegionZ * 32;
		ASSERT((RelChunkX >= 0) && (RelChunkX < 32));
		ASSERT((RelChunkZ >= 0) && (RelChunkZ < 32));

		// Get the chunk data location:
		UInt32 chunkOffset = m_Header[RelChunkX + 32 * RelChunkZ] >> 8;
		UInt32 numChunkSectors = m_Header[RelChunkX + 32 * RelChunkZ] & 0xff;
		if ((chunkOffset < 2) || (numChunkSectors == 0))
		{
			return "";
		}

		// Get the real data size:
		const char * chunkData = m_FileData.data() + chunkOffset * 4096;
		UInt32 chunkSize = GetBEInt(chunkData);
		if ((chunkSize < 2) || (chunkSize / 4096 > numChunkSectors))
		{
			// Bad data, bail out
			return "";
		}

		// Check the compression method:
		if (chunkData[4] != 2)
		{
			// Chunk is in an unknown compression
			return "";
		}
		chunkSize--;

		// Read the chunk data:
		return m_FileData.substr(chunkOffset * 4096 + 5, chunkSize);
	}

protected:
	AString m_FileData;
	UInt32 m_Header[2048];


	/** Reads the whole specified file contents and parses the header. */
	void readFile(const AString & a_FileName)
	{
		// Read the entire file:
		m_FileData = cFile::ReadWholeFile(a_FileName);
		if (m_FileData.size() < sizeof(m_Header))
		{
			return;
		}

		// Parse the header - change endianness:
		const char * hdr = m_FileData.data();
		for (size_t i = 0; i < ARRAYCOUNT(m_Header); i++)
		{
			m_Header[i] = GetBEInt(hdr + 4 * i);
		}
		m_IsValid = true;
	}
};





////////////////////////////////////////////////////////////////////////////////
// AnvilSource:

AnvilSource::AnvilSource(QString a_WorldRegionFolder) :
	m_WorldRegionFolder(a_WorldRegionFolder)
{
}





void AnvilSource::getChunkBiomes(int a_ChunkX, int a_ChunkZ, Chunk & a_DestChunk)
{
	// Load the compressed data:
	AString compressedChunkData = getCompressedChunkData(a_ChunkX, a_ChunkZ);
	if (compressedChunkData.empty())
	{
		return;
	}

	// Uncompress the chunk data:
	AString uncompressed;
	int res = InflateString(compressedChunkData.data(), compressedChunkData.size(), uncompressed);
	if (res != Z_OK)
	{
		return;
	}

	// Parse the NBT data:
	cParsedNBT nbt(uncompressed.data(), uncompressed.size());
	if (!nbt.IsValid())
	{
		return;
	}

	// Get the biomes out of the NBT:
	int Level = nbt.FindChildByName(0, "Level");
	if (Level < 0)
	{
		return;
	}
	cChunkDef::BiomeMap biomeMap;
	int mcsBiomes = nbt.FindChildByName(Level, "MCSBiomes");
	if ((mcsBiomes >= 0) && (nbt.GetDataLength(mcsBiomes) == sizeof(biomeMap)))
	{
		// Convert the biomes from BigEndian to platform native numbers:
		const char * beBiomes = nbt.GetData(mcsBiomes);
		for (size_t i = 0; i < ARRAYCOUNT(biomeMap); i++)
		{
			biomeMap[i] = (EMCSBiome)GetBEInt(beBiomes + 4 * i);
		}
		a_DestChunk.setBiomes(biomeMap);
		return;
	}

	// MCS biomes not found, load Vanilla biomes instead:
	int biomes = nbt.FindChildByName(Level, "Biomes");
	if ((biomes < 0) || (nbt.GetDataLength(biomes) != ARRAYCOUNT(biomeMap)))
	{
		return;
	}
	// Convert the biomes from Vanilla to EMCSBiome:
	const char * vanillaBiomes = nbt.GetData(biomes);
	for (size_t i = 0; i < ARRAYCOUNT(biomeMap); i++)
	{
		biomeMap[i] = EMCSBiome(vanillaBiomes[i]);
	}
	a_DestChunk.setBiomes(biomeMap);
}





void AnvilSource::reload()
{
	// Remove all files from the cache:
	QMutexLocker lock(&m_Mtx);
	m_Files.clear();
}





void AnvilSource::chunkToRegion(int a_ChunkX, int a_ChunkZ, int & a_RegionX, int & a_RegionZ)
{
	a_RegionX = a_ChunkX >> 5;
	a_RegionZ = a_ChunkZ >> 5;
}





AString AnvilSource::getCompressedChunkData(int a_ChunkX, int a_ChunkZ)
{
	return getAnvilFile(a_ChunkX, a_ChunkZ)->getChunkData(a_ChunkX, a_ChunkZ);
}





AnvilSource::AnvilFilePtr AnvilSource::getAnvilFile(int a_ChunkX, int a_ChunkZ)
{
	int RegionX, RegionZ;
	chunkToRegion(a_ChunkX, a_ChunkZ, RegionX, RegionZ);

	// Search the cache for the file:
	QMutexLocker lock(&m_Mtx);
	for (auto itr = m_Files.begin(), end = m_Files.end(); itr != end; ++itr)
	{
		if (((*itr)->m_RegionX == RegionX) && ((*itr)->m_RegionZ == RegionZ))
		{
			// Found the file in the cache, move it to front and return it:
			AnvilFilePtr file(*itr);
			m_Files.erase(itr);
			m_Files.push_front(file);
			return file;
		}
	}

	// File not in cache, create it:
	AnvilFilePtr file(new AnvilFile(RegionX, RegionZ, m_WorldRegionFolder.toStdString()));
	m_Files.push_front(file);
	return file;
}





