
// GZipFile.h

// Declares the GZipFile namespace representing a wrapper over a file stream that can read and write to GZip'd files





#pragma once

#include "StringCompression.h"





namespace GZipFile
{
	/** Reads the rest of the file and returns the decompressed contents. */
	Compression::Result ReadRestOfFile(const std::string & a_FileName);

	/** Writes a_Contents into file, compressing it along the way. */
	void Write(const std::string & a_FileName, ContiguousByteBufferView a_Contents);
} ;
