
// StringCompression.cpp

// Implements the wrapping functions for compression and decompression using AString as their data

#include "Globals.h"
#include "StringCompression.h"
#include "zlib.h"





/// Compresses a_Data into a_Compressed; return Z_XXX error constants same as zlib's compress2()
int CompressString(const char * a_Data, int a_Length, AString & a_Compressed)
{
	uLongf CompressedSize = compressBound(a_Length);
	
	// HACK: We're assuming that AString returns its internal buffer in its data() call and we're overwriting that buffer!
	// It saves us one allocation and one memcpy of the entire compressed data
	// It may not work on some STL implementations! (Confirmed working on MSVC 2008 & 2010)
	a_Compressed.resize(CompressedSize);
	int errorcode = compress2( (Bytef*)a_Compressed.data(), &CompressedSize, (const Bytef*)a_Data, a_Length, Z_DEFAULT_COMPRESSION);
	if (errorcode != Z_OK)
	{
		return errorcode;
	}
	a_Compressed.resize(CompressedSize);
	return Z_OK;
}





/// Uncompresses a_Data into a_Decompressed; returns Z_XXX error constants same as zlib's uncompress()
int UncompressString(const char * a_Data, int a_Length, AString & a_Uncompressed, int a_UncompressedSize)
{
	// HACK: We're assuming that AString returns its internal buffer in its data() call and we're overwriting that buffer!
	// It saves us one allocation and one memcpy of the entire compressed data
	// It may not work on some STL implementations! (Confirmed working on MSVC 2008 & 2010)
	a_Uncompressed.resize(a_UncompressedSize);
	int errorcode = uncompress((Bytef*)a_Uncompressed.data(), (uLongf *)&a_UncompressedSize, (const Bytef*)a_Data, a_Length);
	if (errorcode != Z_OK)
	{
		return errorcode;
	}
	a_Uncompressed.resize(a_UncompressedSize);
	return Z_OK;
}




