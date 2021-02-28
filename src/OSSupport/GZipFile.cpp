
// GZipFile.cpp

// Implements the cGZipFile class representing a RAII wrapper over zlib's GZip file routines

#include "Globals.h"
#include "File.h"
#include "GZipFile.h"





Compression::Result GZipFile::ReadRestOfFile(const std::string & a_FileName)
{
	InputFileStream File(a_FileName, InputFileStream::binary);
	const std::string Input{ std::istreambuf_iterator<char>(File), std::istreambuf_iterator<char>() };
	const ContiguousByteBufferView Data{ reinterpret_cast<const std::byte *>(Input.data()), Input.size() };

	return Compression::Extractor().ExtractGZip(Data);
}





void GZipFile::Write(const std::string & a_FileName, ContiguousByteBufferView a_Contents)
{
	OutputFileStream(a_FileName, OutputFileStream::binary) << Compression::Compressor().CompressGZip(a_Contents).GetStringView();
}
