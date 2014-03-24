/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// StringCompression.h

// Interfaces to the wrapping functions for compression and decompression using AString as their data

#include "zlib/zlib.h"  // Needed for the Z_XXX return values





/// Compresses a_Data into a_Compressed using ZLIB; returns Z_XXX error constants same as zlib's compress2()
extern int CompressString(const char * a_Data, int a_Length, AString & a_Compressed, int a_Factor);

/// Uncompresses a_Data into a_Uncompressed; returns Z_XXX error constants same as zlib's decompress()
extern int UncompressString(const char * a_Data, int a_Length, AString & a_Uncompressed, int a_UncompressedSize);

/// Compresses a_Data into a_Compressed using GZIP; returns Z_OK for success or Z_XXX error constants same as zlib
extern int CompressStringGZIP(const char * a_Data, int a_Length, AString & a_Compressed);

/// Uncompresses a_Data into a_Uncompressed using GZIP; returns Z_OK for success or Z_XXX error constants same as zlib
extern int UncompressStringGZIP(const char * a_Data, int a_Length, AString & a_Uncompressed);



