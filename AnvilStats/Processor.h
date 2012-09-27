
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
		
		// cIsThread override:
		virtual void Execute(void) override;
		
		void ProcessFile(const AString & a_FileName);
		void ProcessChunk(cFile & a_File, int a_ChunkX, int a_ChunkZ, unsigned a_SectorStart, unsigned a_SectorSize, unsigned a_TimeStamp);
		void ProcessCompressedChunkData(int a_ChunkX, int a_ChunkZ, const char * a_CompressedData, int a_CompressedSize);
		void ProcessParsedChunkData(int a_ChunkX, int a_ChunkZ, cParsedNBT & a_NBT);
		bool ProcessChunkSections(int a_ChunkX, int a_ChunkZ, cParsedNBT & a_NBT, int a_LevelTag);

	public:
		cThread(cCallback & a_Callback, cProcessor & a_ParentProcessor);
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
	
	void PopulateFileQueue(const AString & a_WorldFolder);
	
	AString GetOneFileName(void);
} ;




