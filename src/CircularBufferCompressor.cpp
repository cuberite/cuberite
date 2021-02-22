
#include "Globals.h"
#include "CircularBufferCompressor.h"
#include "ByteBuffer.h"





ContiguousByteBufferView CircularBufferCompressor::GetView() const
{
	return m_ContiguousIntermediate;
}





Compression::Result CircularBufferCompressor::Compress()
{
	return m_Compressor.CompressZLib(m_ContiguousIntermediate);
}





void CircularBufferCompressor::ReadFrom(cByteBuffer & Buffer)
{
	Buffer.ReadAll(m_ContiguousIntermediate);
}





void CircularBufferCompressor::ReadFrom(cByteBuffer & Buffer, size_t Size)
{
	Buffer.ReadSome(m_ContiguousIntermediate, Size);
}





ContiguousByteBufferView CircularBufferExtractor::GetView() const
{
	return m_ContiguousIntermediate;
}





Compression::Result CircularBufferExtractor::Extract(size_t UncompressedSize)
{
	return m_Extractor.ExtractZLib(m_ContiguousIntermediate, UncompressedSize);
}





void CircularBufferExtractor::ReadFrom(cByteBuffer & Buffer, size_t Size)
{
	Buffer.ReadSome(m_ContiguousIntermediate, Size);
}
