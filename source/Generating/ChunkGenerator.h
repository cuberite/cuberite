
// ChunkGenerator.h

// Interfaces to the cChunkGenerator class representing the thread that generates chunks

/*
The object takes requests for generating chunks and processes them in a separate thread one by one.
The requests are not added to the queue if there is already a request with the same coords
Before generating, the thread checks if the chunk hasn't been already generated.
It is theoretically possible to have multiple generator threads by having multiple instances of this object,
but then it MAY happen that the chunk is generated twice.
If the generator queue is overloaded, the generator skips chunks with no clients in them
*/





#pragma once

#include "../OSSupport/IsThread.h"
#include "../ChunkDef.h"





// fwd:
class cWorld;
class cIniFile;
class cChunkDesc;





class cChunkGenerator :
	cIsThread
{
	typedef cIsThread super;
	
public:
	/// The interface that a class has to implement to become a generator
	class cGenerator
	{
	public:
		cGenerator(cChunkGenerator & a_ChunkGenerator);
		virtual ~cGenerator() {} ;  // Force a virtual destructor

		/// Called to initialize the generator on server startup.
		virtual void Initialize(cWorld * a_World, cIniFile & a_IniFile);
		
		/// Generates the biomes for the specified chunk (directly, not in a separate thread). Used by the world loader if biomes failed loading.
		virtual void GenerateBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) = 0;

		/// Returns the biome at the specified coords. Used by ChunkMap if an invalid chunk is queried for biome. Default implementation uses GenerateBiomes().
		virtual EMCSBiome GetBiomeAt(int a_BlockX, int a_BlockZ);

		/// Called in a separate thread to do the actual chunk generation. Generator should generate into a_ChunkDesc.
		virtual void DoGenerate(int a_ChunkX, int a_ChunkZ, cChunkDesc & a_ChunkDesc) = 0;
		
	protected:
		cChunkGenerator & m_ChunkGenerator;
		cWorld          * m_World;
	} ;
	

	cChunkGenerator (void);
	~cChunkGenerator();

	bool Start(cWorld * a_World, cIniFile & a_IniFile);
	void Stop(void);

	/// Queues the chunk for generation; removes duplicate requests
	void QueueGenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Generates the biomes for the specified chunk (directly, not in a separate thread). Used by the world loader if biomes failed loading.
	void GenerateBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap);
	
	void WaitForQueueEmpty(void);
	
	int GetQueueLength(void);
	
	int GetSeed(void) const { return m_Seed; }
	
	/// Returns the biome at the specified coords. Used by ChunkMap if an invalid chunk is queried for biome
	EMCSBiome GetBiomeAt(int a_BlockX, int a_BlockZ);

	/// Reads a block type from the ini file; returns the blocktype on success, emits a warning and returns a_Default's representation on failure.
	static BLOCKTYPE GetIniBlock(cIniFile & a_IniFile, const AString & a_SectionName, const AString & a_ValueName, const AString & a_Default);
	
private:

	cWorld * m_World;
	
	int m_Seed;

	cCriticalSection m_CS;
	cChunkCoordsList m_Queue;
	cEvent           m_Event;       ///< Set when an item is added to the queue or the thread should terminate
	cEvent           m_evtRemoved;  ///< Set when an item is removed from the queue
	
	cGenerator * m_Generator;  ///< The actual generator engine used to generate chunks

	// cIsThread override:
	virtual void Execute(void) override;

	void DoGenerate(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
};




