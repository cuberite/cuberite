
// ChunkExtract.cpp

// Implements the cChunkExtract class representing a cCallback descendant that extracts raw chunk data into separate .chunk files

#include "Globals.h"
#include "ChunkExtract.h"
#include "../../src/OSSupport/GZipFile.h"





cChunkExtract::cChunkExtract(const AString & iWorldFolder) :
	mWorldFolder(iWorldFolder)
{
}





bool cChunkExtract::OnNewChunk(int a_ChunkX, int a_ChunkZ)
{
	int AnvilX = (a_ChunkX - ((a_ChunkX > 0) ? 0 : 31)) / 32;
	int AnvilZ = (a_ChunkZ - ((a_ChunkZ > 0) ? 0 : 31)) / 32;
	if ((AnvilX != mCurAnvilX) || (AnvilZ != mCurAnvilZ))
	{
		OpenAnvilFile(AnvilX, AnvilZ);
	}
	mCurChunkX = a_ChunkX;
	mCurChunkZ = a_ChunkZ;
	return false;
}





bool cChunkExtract::OnCompressedDataSizePos(int a_CompressedDataSize, int a_DataOffset, char a_CompressionMethod)
{
	if (!mAnvilFile.IsOpen())
	{
		return true;
	}
	cFile ChunkFile;
	AString ChunkPath = Printf("%d.%d.zchunk", mCurChunkX, mCurChunkZ);
	if (!ChunkFile.Open(ChunkPath, cFile::fmWrite))
	{
		LOG("Cannot open zchunk file \"%s\" for writing. Chunk [%d, %d] skipped.", ChunkPath.c_str(), mCurChunkX, mCurChunkZ);
		return false;
	}

	// Copy data from mAnvilFile to ChunkFile:
	mAnvilFile.Seek(a_DataOffset);
	for (int BytesToCopy = a_CompressedDataSize; BytesToCopy > 0;)
	{
		char Buffer[64000];
		int NumBytes = std::min(BytesToCopy, (int)sizeof(Buffer));
		int BytesRead = mAnvilFile.Read(Buffer, NumBytes);
		if (BytesRead != NumBytes)
		{
			LOG("Cannot copy chunk data, chunk [%d, %d] is probably corrupted. Skipping chunk.", mCurChunkX, mCurChunkZ);
			return false;
		}
		ChunkFile.Write(Buffer, BytesRead);
		BytesToCopy -= BytesRead;
	}  // for BytesToCopy
	return false;
}





bool cChunkExtract::OnDecompressedData(const char * a_DecompressedNBT, int a_DataSize)
{
	ASSERT(mAnvilFile.IsOpen());  // If it weren't, the OnCompressedDataSizePos would've prevented this from running
	AString FileName = Printf("%d.%d.gzchunk", mCurChunkX, mCurChunkZ);
	cGZipFile GZipChunk;
	if (!GZipChunk.Open(FileName, cGZipFile::fmWrite))
	{
		LOG("Cannot open gzchunk file \"%s\" for writing. Chunk [%d, %d] skipped.", FileName.c_str(), mCurChunkX, mCurChunkZ);
		return true;
	}
	GZipChunk.Write(a_DecompressedNBT, a_DataSize);
	return true;
}





void cChunkExtract::OpenAnvilFile(int a_AnvilX, int a_AnvilZ)
{
	mAnvilFile.Close();
	AString FileName = Printf("%s/r.%d.%d.mca", mWorldFolder.c_str(), a_AnvilX, a_AnvilZ);
	if (!mAnvilFile.Open(FileName, cFile::fmRead))
	{
		LOG("Cannot open Anvil file \"%s\" for reading", FileName.c_str());
	}
	mCurAnvilX = a_AnvilX;
	mCurAnvilZ = a_AnvilZ;
}
