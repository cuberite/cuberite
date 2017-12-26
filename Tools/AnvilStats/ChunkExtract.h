
// ChunkExtract.h

// Declares the cChunkExtract class representing a cCallback descendant that extracts raw chunk data into separate .chunk files





#pragma once

#include "Callback.h"





class cChunkExtract :
	public cCallback
{
public:
	cChunkExtract(const AString & iWorldFolder);

protected:
	AString mWorldFolder;
	cFile   mAnvilFile;
	int     mCurAnvilX;  // X-coord of mAnvilFile, in Anvil-coords (1 Anvil-coord = 32 chunks)
	int     mCurAnvilZ;  // Z-coord of mAnvilFile, -"-
	int     mCurChunkX;  // X-coord of the chunk being processed
	int     mCurChunkZ;  // Z-coord of the chunk being processed

	/** Opens new anvil file into mAnvilFile, sets mCurAnvilX and mCurAnvilZ */
	void OpenAnvilFile(int a_AnvilX, int a_AnvilZ);

	// cCallback overrides:
	virtual bool OnNewChunk(int a_ChunkX, int a_ChunkZ) override;
	virtual bool OnHeader(int a_FileOffset, unsigned char a_NumSectors, int a_Timestamp) override { return false; }
	virtual bool OnCompressedDataSizePos(int a_CompressedDataSize, int a_DataOffset, char a_CompressionMethod) override;
	virtual bool OnDecompressedData(const char * a_DecompressedNBT, int a_DataSize) override;
} ;





class cChunkExtractFactory :
	public cCallbackFactory
{
public:
	cChunkExtractFactory(const AString & iWorldFolder) :
		mWorldFolder(iWorldFolder)
	{
	}

	virtual cCallback * CreateNewCallback(void) override
	{
		return new cChunkExtract(mWorldFolder);
	}

protected:
	AString mWorldFolder;
} ;
