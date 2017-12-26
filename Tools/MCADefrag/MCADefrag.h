
// MCADefrag.h

// Interfaces to the cMCADefrag class encapsulating the entire app





#pragma once






class cMCADefrag
{
public:
	enum
	{
		MAX_COMPRESSED_CHUNK_DATA_SIZE = (1 MiB),
		MAX_RAW_CHUNK_DATA_SIZE        = (100 MiB),
	} ;

	cMCADefrag(void);

	/** Reads the cmdline params and initializes the app.
	Returns true if the app should continue, false if not. */
	bool Init(int argc, char ** argv);

	/** Runs the entire app. */
	void Run(void);

protected:
	/** A single thread processing MCA files from the queue */
	class cThread :
		public cIsThread
	{
		typedef cIsThread super;

	public:
		cThread(cMCADefrag & a_Parent);

	protected:
		/** The compression methods, as specified by the MCA compression method byte. */
		enum
		{
			COMPRESSION_GZIP = 1,
			COMPRESSION_ZLIB = 2,
		} ;


		cMCADefrag & m_Parent;

		/** The current compressed chunk data. Valid after a successful ReadChunk().
		This contains only the compression method byte and the compressed data,
		but not the exact-length preceding the data in the MCA file. */
		unsigned char m_CompressedChunkData[MAX_COMPRESSED_CHUNK_DATA_SIZE];

		/** Size of the actual current compressed chunk data, excluding the 4 exact-length bytes.
		This is the amount of bytes in m_CompressedChunkData[] that are valid. */
		int m_CompressedChunkDataSize;

		/** The current raw chunk data. Valid after a successful ReadChunk(), if recompression is active. */
		unsigned char m_RawChunkData[MAX_RAW_CHUNK_DATA_SIZE];

		/** Size of the actual current raw chunk data. */
		int m_RawChunkDataSize;

		/** Number of the sector where the next chunk will be written by WriteChunk(). */
		int m_CurrentSectorOut;

		/** Set to true when the chunk has been successfully uncompressed. Only used if recompression is active.
		WriteChunk() tests this flag to decide whether to call Compress(). */
		bool m_IsChunkUncompressed;


		/** Processes the specified file. */
		void ProcessFile(const AString & a_FileName);

		/** Reads the chunk data into m_CompressedChunkData.
		Calls DecompressChunkData() if recompression is active.
		a_LocationRaw is the pointer to the first byte of the Location data in the MCA header.
		Returns true if successful. */
		bool ReadChunk(cFile & a_File, const Byte * a_LocationRaw);

		/** Writes the chunk data from m_CompressedData or m_RawChunkData (performing recompression) into file.
		Calls CompressChunkData() for the actual compression, if recompression is active.
		a_LocationRaw is the pointer to the first byte of the Location data to be put into the MCA header,
		the chunk's location is stored in that memory area. Updates m_CurrentSectorOut.
		Returns true if successful. */
		bool WriteChunk(cFile & a_File, Byte * a_LocationRaw);

		/** Uncompresses the chunk data from m_CompressedChunkData into m_RawChunkData.
		Returns true if successful, false on failure. */
		bool UncompressChunk(void);

		/** Uncompresses the chunk data from m_CompressedChunkData into m_RawChunkData, using Gzip.
		Returns true if successful, false on failure. */
		bool UncompressChunkGzip(void);

		/** Uncompresses the chunk data from m_CompressedChunkData into m_RawChunkData, using Zlib.
		Returns true if successful, false on failure. */
		bool UncompressChunkZlib(void);

		/** Compresses the chunk data from m_RawChunkData into m_CompressedChunkData.
		Returns true if successful, false on failure. */
		bool CompressChunk(void);

		// cIsThread overrides:
		virtual void Execute(void) override;
	} ;

	typedef std::list<cThread *> cThreads;


	/** The mutex protecting m_Files agains multithreaded access. */
	cCriticalSection m_CS;

	/** The queue of MCA files to be processed by the threads. Protected by m_CS. */
	AStringVector m_Queue;

	/** List of threads that the server has running. */
	cThreads m_Threads;

	/** The number of threads that should be started. Configurable on the command line. */
	int m_NumThreads;

	/** If set to true, the chunk data is recompressed while saving each MCA file. */
	bool m_ShouldRecompress;


	/** Starts a new processing thread and adds it to cThreads. */
	void StartThread(void);

	/** Retrieves one file from the queue (and removes it from the queue).
	Returns an empty string when queue empty. */
	AString GetNextFileName(void);
} ;
