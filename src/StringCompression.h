
// StringCompression.h

// Interfaces to the wrapping functions for compression and decompression using AString as their data

#include "zlib/zlib.h"  // Needed for the Z_XXX return values





/** Compresses a_Data into a_Compressed using ZLIB; returns Z_XXX error constants same as zlib's compress2() */
extern int CompressString(const char * a_Data, size_t a_Length, AString & a_Compressed, int a_Factor);

/** Uncompresses a_Data into a_Uncompressed; returns Z_XXX error constants same as zlib's decompress() */
extern int UncompressString(const char * a_Data, size_t a_Length, AString & a_Uncompressed, size_t a_UncompressedSize);

/** Compresses a_Data into a_Compressed using GZIP; returns Z_OK for success or Z_XXX error constants same as zlib */
extern int CompressStringGZIP(const char * a_Data, size_t a_Length, AString & a_Compressed);

/** Uncompresses a_Data into a_Uncompressed using GZIP; returns Z_OK for success or Z_XXX error constants same as zlib */
extern int UncompressStringGZIP(const char * a_Data, size_t a_Length, AString & a_Uncompressed);

/** Uncompresses a_Data into a_Uncompressed using Inflate; returns Z_OK for success or Z_XXX error constants same as zlib */
extern int InflateString(const char * a_Data, size_t a_Length, AString & a_Uncompressed);


