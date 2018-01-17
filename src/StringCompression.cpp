
// StringCompression.cpp

// Implements the wrapping functions for compression and decompression using AString as their data

#include "Globals.h"
#include "StringCompression.h"





int CompressString(const char * a_Data, size_t a_Length, AString & a_Compressed, int a_Factor)
{
	uLongf CompressedSize = compressBound(static_cast<uLong>(a_Length));

	// HACK: We're assuming that AString returns its internal buffer in its data() call and we're overwriting that buffer!
	// It saves us one allocation and one memcpy of the entire compressed data
	// It may not work on some STL implementations! (Confirmed working on all currently used MSVC, GCC and Clang versions)
	a_Compressed.resize(CompressedSize);
	int errorcode = compress2(reinterpret_cast<Bytef *>(const_cast<char *>(a_Compressed.data())), &CompressedSize, reinterpret_cast<const Bytef *>(a_Data), static_cast<uLong>(a_Length), a_Factor);
	if (errorcode != Z_OK)
	{
		return errorcode;
	}
	a_Compressed.resize(CompressedSize);
	return Z_OK;
}





int UncompressString(const char * a_Data, size_t a_Length, AString & a_Uncompressed, size_t a_UncompressedSize)
{
	// HACK: We're assuming that AString returns its internal buffer in its data() call and we're overwriting that buffer!
	// It saves us one allocation and one memcpy of the entire compressed data
	// It may not work on some STL implementations! (Confirmed working on all currently used MSVC, GCC and Clang versions)
	a_Uncompressed.resize(a_UncompressedSize);
	uLongf UncompressedSize = static_cast<uLongf>(a_UncompressedSize);  // On some architectures the uLongf is different in size to int, that may be the cause of the -5 error
	int errorcode = uncompress(reinterpret_cast<Bytef *>(const_cast<char *>(a_Uncompressed.data())), &UncompressedSize, reinterpret_cast<const Bytef *>(a_Data), static_cast<uLong>(a_Length));
	if (errorcode != Z_OK)
	{
		return errorcode;
	}
	a_Uncompressed.resize(UncompressedSize);
	return Z_OK;
}





int CompressStringGZIP(const char * a_Data, size_t a_Length, AString & a_Compressed)
{
	// Compress a_Data into a_Compressed using GZIP; return Z_XXX error constants same as zlib's compress2()

	a_Compressed.reserve(a_Length);

	char Buffer[64 KiB];
	z_stream strm;
	memset(&strm, 0, sizeof(strm));
	strm.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(a_Data));
	strm.avail_in = static_cast<uInt>(a_Length);
	strm.next_out = reinterpret_cast<Bytef *>(Buffer);
	strm.avail_out = sizeof(Buffer);

	int res = deflateInit2(&strm, 9, Z_DEFLATED, 31, 9, Z_DEFAULT_STRATEGY);
	if (res != Z_OK)
	{
		LOG("%s: compression initialization failed: %d (\"%s\").", __FUNCTION__, res, strm.msg);
		return res;
	}

	for (;;)
	{
		res = deflate(&strm, Z_FINISH);
		switch (res)
		{
			case Z_OK:
			{
				// Some data has been compressed. Consume the buffer and continue compressing
				a_Compressed.append(Buffer, sizeof(Buffer) - strm.avail_out);
				strm.next_out = reinterpret_cast<Bytef *>(Buffer);
				strm.avail_out = sizeof(Buffer);
				if (strm.avail_in == 0)
				{
					// All data has been compressed
					deflateEnd(&strm);
					return Z_OK;
				}
				break;
			}

			case Z_STREAM_END:
			{
				// Finished compressing. Consume the rest of the buffer and return
				a_Compressed.append(Buffer, sizeof(Buffer) - strm.avail_out);
				deflateEnd(&strm);
				return Z_OK;
			}

			default:
			{
				// An error has occurred, log it and return the error value
				LOG("%s: compression failed: %d (\"%s\").", __FUNCTION__, res, strm.msg);
				deflateEnd(&strm);
				return res;
			}
		}  // switch (res)
	}  // while (true)
}





extern int UncompressStringGZIP(const char * a_Data, size_t a_Length, AString & a_Uncompressed)
{
	// Uncompresses a_Data into a_Uncompressed using GZIP; returns Z_OK for success or Z_XXX error constants same as zlib

	a_Uncompressed.reserve(a_Length);

	char Buffer[64 KiB];
	z_stream strm;
	memset(&strm, 0, sizeof(strm));
	strm.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(a_Data));
	strm.avail_in = static_cast<uInt>(a_Length);
	strm.next_out = reinterpret_cast<Bytef *>(Buffer);
	strm.avail_out = sizeof(Buffer);

	int res = inflateInit2(&strm, 31);  // Force GZIP decoding
	if (res != Z_OK)
	{
		LOG("%s: uncompression initialization failed: %d (\"%s\").", __FUNCTION__, res, strm.msg);
		return res;
	}

	for (;;)
	{
		res = inflate(&strm, Z_NO_FLUSH);
		switch (res)
		{
			case Z_OK:
			{
				// Some data has been uncompressed. Consume the buffer and continue uncompressing
				a_Uncompressed.append(Buffer, sizeof(Buffer) - strm.avail_out);
				strm.next_out = reinterpret_cast<Bytef *>(Buffer);
				strm.avail_out = sizeof(Buffer);
				if (strm.avail_in == 0)
				{
					// All data has been uncompressed
					inflateEnd(&strm);
					return Z_OK;
				}
				break;
			}

			case Z_STREAM_END:
			{
				// Finished uncompressing. Consume the rest of the buffer and return
				a_Uncompressed.append(Buffer, sizeof(Buffer) - strm.avail_out);
				inflateEnd(&strm);
				return Z_OK;
			}

			default:
			{
				// An error has occurred, log it and return the error value
				LOG("%s: uncompression failed: %d (\"%s\").", __FUNCTION__, res, strm.msg);
				inflateEnd(&strm);
				return res;
			}
		}  // switch (res)
	}  // while (true)
}





extern int InflateString(const char * a_Data, size_t a_Length, AString & a_Uncompressed)
{
	a_Uncompressed.reserve(a_Length);

	char Buffer[64 KiB];
	z_stream strm;
	memset(&strm, 0, sizeof(strm));
	strm.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(a_Data));
	strm.avail_in = static_cast<uInt>(a_Length);
	strm.next_out = reinterpret_cast<Bytef *>(Buffer);
	strm.avail_out = sizeof(Buffer);

	int res = inflateInit(&strm);  // Force GZIP decoding
	if (res != Z_OK)
	{
		LOG("%s: inflation initialization failed: %d (\"%s\").", __FUNCTION__, res, strm.msg);
		return res;
	}

	for (;;)
	{
		res = inflate(&strm, Z_NO_FLUSH);
		switch (res)
		{
			case Z_OK:
			{
				// Some data has been uncompressed. Consume the buffer and continue uncompressing
				a_Uncompressed.append(Buffer, sizeof(Buffer) - strm.avail_out);
				strm.next_out = reinterpret_cast<Bytef *>(Buffer);
				strm.avail_out = sizeof(Buffer);
				if (strm.avail_in == 0)
				{
					// All data has been uncompressed
					inflateEnd(&strm);
					return Z_OK;
				}
				break;
			}

			case Z_STREAM_END:
			{
				// Finished uncompressing. Consume the rest of the buffer and return
				a_Uncompressed.append(Buffer, sizeof(Buffer) - strm.avail_out);
				inflateEnd(&strm);
				return Z_OK;
			}

			default:
			{
				// An error has occurred, log it and return the error value
				LOG("%s: inflation failed: %d (\"%s\").", __FUNCTION__, res, strm.msg);
				inflateEnd(&strm);
				return res;
			}
		}  // switch (res)
	}  // while (true)
}




