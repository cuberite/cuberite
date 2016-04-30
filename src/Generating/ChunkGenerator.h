
// ChunkGenerator.h

// Interfaces to the cChunkGenerator class representing the thread that generates chunks

/*
The object takes requests for generating chunks and processes them in a separate thread one by one.
The requests are not added to the queue if there is already a request with the same coords
Before generating, the thread checks if the chunk hasn't been already generated.
It is theoretically possible to have multiple generator threads by having multiple instances of this object,
but then it MAY happen that the chunk is generated twice.
*/





#pragma once

#include "../OSSupport/IsThread.h"
#include "../ChunkDef.h"





// fwd:
class cIniFile;
class cChunkDesc;





class cChunkGenerator :
	cIsThread
{
	typedef cIsThread super;

public:
	/** The interface that a class has to implement to become a generator */
	class cGenerator
	{
	public:
		cGenerator(cChunkGenerator & a_ChunkGenerator);
		virtual ~cGenerator() {}  // Force a virtual destructor

		/** Called to initialize the generator on server startup. */
		virtual void Initialize(cIniFile & a_IniFile);

		/** Generates the biomes for the specified chunk (directly, not in a separate thread). Used by the world loader if biomes failed loading. */
		virtual void GenerateBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) = 0;

		/** Returns the biome at the specified coords. Used by ChunkMap if an invalid chunk is queried for biome. Default implementation uses GenerateBiomes(). */
		virtual EMCSBiome GetBiomeAt(int a_BlockX, int a_BlockZ);

		/** Called in a separate thread to do the actual chunk generation. Generator should generate into a_ChunkDesc. */
		virtual void DoGenerate(int a_ChunkX, int a_ChunkZ, cChunkDesc & a_ChunkDesc) = 0;

	protected:
		cChunkGenerator & m_ChunkGenerator;
	} ;


	/** The interface through which the plugins are called for their OnChunkGenerating / OnChunkGenerated hooks. */
	class cPluginInterface
	{
	public:
		// Force a virtual destructor
		virtual ~cPluginInterface() {}

		/** Called when the chunk is about to be generated.
		The generator may be partly or fully overriden by the implementation. */
		virtual void CallHookChunkGenerating(cChunkDesc & a_ChunkDesc) = 0;

		/** Called after the chunk is generated, before it is handed to the chunk sink.
		a_ChunkDesc contains the generated chunk data. Implementation may modify this data. */
		virtual void CallHookChunkGenerated(cChunkDesc & a_ChunkDesc) = 0;
	} ;


	/** The interface through which the generated chunks are handed to the cWorld or whoever created us. */
	class cChunkSink
	{
	public:
		// Force a virtual destructor
		virtual ~cChunkSink() {}

		/** Called after the chunk has been generated
		The interface may store the chunk, send it over network, whatever.
		The chunk is not expected to be modified, but the generator will survive if the implementation
		changes the data within. All changes are ignored, though. */
		virtual void OnChunkGenerated(cChunkDesc & a_ChunkDesc) = 0;

		/** Called just before the chunk generation is started,
		to verify that it hasn't been generated in the meantime.
		If this callback returns true, the chunk is not generated. */
		virtual bool IsChunkValid(int a_ChunkX, int a_ChunkZ) = 0;

		/** Called when the generator is overloaded to skip chunks that are no longer needed.
		If this callback returns false, the chunk is not generated. */
		virtual bool HasChunkAnyClients(int a_ChunkX, int a_ChunkZ) = 0;

		/** Called to check whether the specified chunk is in the queued state. */
		virtual bool IsChunkQueued(int a_ChunkX, int a_ChunkZ) = 0;
	} ;


	cChunkGenerator (void);
	~cChunkGenerator();

	bool Start(cPluginInterface & a_PluginInterface, cChunkSink & a_ChunkSink, cIniFile & a_IniFile);
	void Stop(void);

	/** Queues the chunk for generation
	If a-ForceGenerate is set, the chunk is regenerated even if the data is already present in the chunksink.
	a_Callback is called after the chunk is generated. If the chunk was already present, the callback is still called, even if not regenerating.
	It is legal to set the callback to nullptr, no callback is called then. */
	void QueueGenerateChunk(int a_ChunkX, int a_ChunkZ, bool a_ForceGenerate, cChunkCoordCallback * a_Callback = nullptr);

	/** Generates the biomes for the specified chunk (directly, not in a separate thread). Used by the world loader if biomes failed loading. */
	void GenerateBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap);

	int GetQueueLength(void);

	int GetSeed(void) const { return m_Seed; }

	/** Returns the biome at the specified coords. Used by ChunkMap if an invalid chunk is queried for biome */
	EMCSBiome GetBiomeAt(int a_BlockX, int a_BlockZ);

	/** Reads a block type from the ini file; returns the blocktype on success, emits a warning and returns a_Default's representation on failure. */
	static BLOCKTYPE GetIniBlock(cIniFile & a_IniFile, const AString & a_SectionName, const AString & a_ValueName, const AString & a_Default);

private:

	struct sQueueItem
	{
		/** The chunk coords */
		int m_ChunkX, m_ChunkZ;

		/** Callback to call after generating. */
		cChunkCoordCallback * m_Callback;
	};


	/** Seed used for the generator. */
	int m_Seed;

	/** CS protecting access to the queue. */
	cCriticalSection m_CS;

	/** Queue of the chunks to be generated. Protected against multithreaded access by m_CS. */
	std::vector<sQueueItem> m_Queue;

	/** Set when an item is added to the queue or the thread should terminate. */
	cEvent m_Event;

	/** Set when an item is removed from the queue. */
	cEvent m_evtRemoved;

	/** The actual generator engine used to generate chunks. */
	cGenerator * m_Generator;

	/** The plugin interface that may modify the generated chunks */
	cPluginInterface * m_PluginInterface;

	/** The destination where the generated chunks are sent */
	cChunkSink * m_ChunkSink;


	// cIsThread override:
	virtual void Execute(void) override;

	/** Generates the specified chunk and sets it into the chunksink. */
	void DoGenerate(int a_ChunkX, int a_ChunkZ);
};




