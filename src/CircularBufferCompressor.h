
#pragma once

#include "StringCompression.h"





class cByteBuffer;





class CircularBufferCompressor
{
public:

	ContiguousByteBufferView GetView() const;

	Compression::Result Compress();
	void ReadFrom(cByteBuffer & Buffer);
	void ReadFrom(cByteBuffer & Buffer, size_t Size);

private:

	Compression::Compressor m_Compressor;
	std::basic_string<std::byte> m_ContiguousIntermediate;
};





class CircularBufferExtractor
{
public:

	ContiguousByteBufferView GetView() const;

	Compression::Result Extract(size_t UncompressedSize);
	void ReadFrom(cByteBuffer & Buffer, size_t Size);

private:

	Compression::Extractor m_Extractor;
	std::basic_string<std::byte> m_ContiguousIntermediate;
};
