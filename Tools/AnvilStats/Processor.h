
// Processor.h

// Interfaces to the cProcessor class representing the overall processor engine that manages threads, calls callbacks etc.




#pragma once





// fwd:
class cCallback;
class cCallbackFactory;
class cParsedNBT;





class cProcessor
{
	class cThread :
		public cIsThread
	{
		typedef cIsThread super;

		cCallback &  m_Callback;
		cProcessor & m_ParentProcessor;
		cEvent m_HasStarted;

		// cIsThread override:
		virtual void Execute(void) override;

		void ProcessFile(const AString & a_FileName);
		void ProcessFileData(const char * a_FileData, size_t a_Size, int a_ChunkBaseX, int a_ChunkBaseZ);
		void ProcessChunk(const char * a_FileData, int a_ChunkX, int a_ChunkZ, unsigned a_SectorStart, unsigned a_SectorSize, unsigned a_TimeStamp);
		void ProcessCompressedChunkData(int a_ChunkX, int a_ChunkZ, const char * a_CompressedData, int a_CompressedSize);
		void ProcessParsedChunkData(int a_ChunkX, int a_ChunkZ, cParsedNBT & a_NBT);

		// The following processing parts return true if they were interrupted by the callback, causing the processing of current chunk to abort
		bool ProcessChunkSections(int a_ChunkX, int a_ChunkZ, cParsedNBT & a_NBT, int a_LevelTag);
		bool ProcessChunkEntities(int a_ChunkX, int a_ChunkZ, cParsedNBT & a_NBT, int a_LevelTag);
		bool ProcessChunkTileEntities(int a_ChunkX, int a_ChunkZ, cParsedNBT & a_NBT, int a_LevelTag);
		bool ProcessChunkTileTicks(int a_ChunkX, int a_ChunkZ, cParsedNBT & a_NBT, int a_LevelTag);

	public:
		cThread(cCallback & a_Callback, cProcessor & a_ParentProcessor);

		/** Waits until the thread starts processing the callback code. */
		void WaitForStart(void);
	} ;

	typedef std::vector<cThread *> cThreads;

public:
	cProcessor(void);
	~cProcessor();

	void ProcessWorld(const AString & a_WorldFolder, cCallbackFactory & a_CallbackFactory);

protected:
	bool m_IsShuttingDown;  // If true, the threads should stop ASAP

	cCriticalSection m_CS;
	AStringList      m_FileQueue;

	cThreads m_Threads;


	/** Populates m_FileQueue with Anvil files from the specified folder. */
	void PopulateFileQueue(const AString & a_WorldFolder);

	/** Returns one filename from m_FileQueue, and removes the name from the queue. */
	AString GetOneFileName(void);
} ;
