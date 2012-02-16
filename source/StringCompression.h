
// StringCompression.h

// Interfaces to the wrapping functions for compression and decompression using AString as their data




/// Compresses a_Data into a_Compressed; return Z_XXX error constants same as zlib's compress2()
extern int CompressString(const char * a_Data, int a_Length, AString & a_Compressed);

/// Uncompresses a_Data into a_Decompressed; returns Z_XXX error constants same as zlib's decompress()
extern int UncompressString(const char * a_Data, int a_Length, AString & a_Uncompressed, int a_UncompressedSize);




