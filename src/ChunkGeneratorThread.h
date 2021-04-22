#pragma once

#include "OSSupport/IsThread.h"
#include "ChunkDef.h"




// fwd:
class cIniFile;
class cChunkDesc;
class cChunkGenerator;





/** Takes requests for generating chunks and processes them in a separate thread one by one.
The requests are not added to the queue if there is already a request with the same coords.
Before generating, the thread checks if the chunk hasn't been already generated.
It is theoretically possible to have multiple generator threads by having multiple instances of this object,
but then it MAY happen that the chunk is generated twice.
If the generator queue is overloaded, the generator skips chunks with no clients in them. */
class cChunkGeneratorThread :
	public cIsThread
{
	using Super = cIsThread;

public:

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
		virtual bool IsChunkValid(cChunkCoords a_Coords) = 0;

		/** Called when the generator is overloaded to skip chunks that are no longer needed.
		If this callback returns false, the chunk is not generated. */
		virtual bool HasChunkAnyClients(cChunkCoords a_Coords) = 0;

		/** Called to check whether the specified chunk is in the queued state.
		Currently used only in Debug-mode asserts. */
		virtual bool IsChunkQueued(cChunkCoords a_Coords) = 0;
	} ;


	cChunkGeneratorThread (void);
	virtual ~cChunkGeneratorThread() override;

	/** Read settings from the ini file and initialize in preperation for being started. */
	bool Initialize(cPluginInterface & a_PluginInterface, cChunkSink & a_ChunkSink, cIniFile & a_IniFile);

	void Stop(void);

	/** Queues the chunk for generation
	If a-ForceGenerate is set, the chunk is regenerated even if the data is already present in the chunksink.
	a_Callback is called after the chunk is generated. If the chunk was already present, the callback is still called, even if not regenerating.
	It is legal to set the callback to nullptr, no callback is called then.
	If the generator becomes overloaded and skips this chunk, the callback is still called. */
	void QueueGenerateChunk(cChunkCoords a_Coords, bool a_ForceRegeneration, cChunkCoordCallback * a_Callback = nullptr);

	/** Generates the biomes for the specified chunk (directly, not in a separate thread). Used by the world loader if biomes failed loading. */
	void GenerateBiomes(cChunkCoords a_Coords, cChunkDef::BiomeMap & a_BiomeMap);

	void WaitForQueueEmpty();

	size_t GetQueueLength() const;

	int GetSeed() const;

	/** Returns the biome at the specified coords. Used by ChunkMap if an invalid chunk is queried for biome */
	EMCSBiome GetBiomeAt(int a_BlockX, int a_BlockZ);


private:

	struct QueueItem
	{
		/** The chunk coords */
		cChunkCoords m_Coords;

		/** Force the regeneration of an already existing chunk */
		bool m_ForceRegeneration;

		/** Callback to call after generating. */
		cChunkCoordCallback * m_Callback;

		QueueItem(cChunkCoords a_Coords, bool a_ForceRegeneration, cChunkCoordCallback * a_Callback):
			m_Coords(a_Coords),
			m_ForceRegeneration(a_ForceRegeneration),
			m_Callback(a_Callback)
		{
		}
	};

	using Queue = std::list<QueueItem>;


	/** CS protecting access to the queue. */
	mutable cCriticalSection m_CS;

	/** Queue of the chunks to be generated. Protected against multithreaded access by m_CS. */
	Queue m_Queue;

	/** Set when an item is added to the queue or the thread should terminate. */
	cEvent m_Event;

	/** Set when an item is removed from the queue. */
	cEvent m_evtRemoved;

	/** The actual chunk generator engine used. */
	std::unique_ptr<cChunkGenerator> m_Generator;

	/** The plugin interface that may modify the generated chunks */
	cPluginInterface * m_PluginInterface;

	/** The destination where the generated chunks are sent */
	cChunkSink * m_ChunkSink;


	// cIsThread override:
	virtual void Execute(void) override;

	/** Generates the specified chunk and sets it into the chunksink. */
	void DoGenerate(cChunkCoords a_Coords);
};




