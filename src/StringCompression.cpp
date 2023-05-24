
// StringCompression.cpp

// Implements the wrapping functions for compression and decompression

#include "Globals.h"
#include "ByteBuffer.h"
#include "StringCompression.h"

#include <libdeflate.h>




std::string_view Compression::Result::GetStringView() const
{
	const auto View = GetView();
	return { reinterpret_cast<const char *>(View.data()), View.size() };
}





ContiguousByteBufferView Compression::Result::GetView() const
{
	// Get a generic std::byte * to what the variant is currently storing:
	return
	{
		std::visit([](const auto & Buffer) -> const std::byte *
		{
			return Buffer.get();
		}, Storage), Size
	};
}





Compression::Compressor::Compressor(int CompressionFactor)
{
	m_Handle = libdeflate_alloc_compressor(CompressionFactor);

	if (m_Handle == nullptr)
	{
		throw std::bad_alloc();
	}
}





Compression::Compressor::~Compressor()
{
	libdeflate_free_compressor(m_Handle);
}





template <auto Algorithm>
Compression::Result Compression::Compressor::Compress(const void * const Input, const size_t Size)
{
	// Allocate space on the heap to write the compression result, increasing in powers of 2.
	// This will either succeed, or except with bad_alloc.

	auto DynamicCapacity = Result::StaticCapacity * 2;
	while (true)
	{
		auto Dynamic = cpp20::make_unique_for_overwrite<Result::Dynamic::element_type[]>(DynamicCapacity);
		const auto BytesWrittenOut = Algorithm(m_Handle, Input, Size, Dynamic.get(), DynamicCapacity);

		if (BytesWrittenOut != 0)
		{
			return { std::move(Dynamic), BytesWrittenOut };
		}

		DynamicCapacity *= 2;
	}
}





Compression::Result Compression::Compressor::CompressGZip(const ContiguousByteBufferView Input)
{
	return Compress<&libdeflate_gzip_compress>(Input.data(), Input.size());
}





Compression::Result Compression::Compressor::CompressZLib(const ContiguousByteBufferView Input)
{
	return Compress<&libdeflate_zlib_compress>(Input.data(), Input.size());
}





Compression::Result Compression::Compressor::CompressZLib(const void * const Input, const size_t Size)
{
	return Compress<&libdeflate_zlib_compress>(Input, Size);
}





Compression::Extractor::Extractor()
{
	m_Handle = libdeflate_alloc_decompressor();

	if (m_Handle == nullptr)
	{
		throw std::bad_alloc();
	}
}





Compression::Extractor::~Extractor()
{
	libdeflate_free_decompressor(m_Handle);
}





Compression::Result Compression::Extractor::ExtractGZip(ContiguousByteBufferView Input)
{
	return Extract<&libdeflate_gzip_decompress>(Input);
}





Compression::Result Compression::Extractor::ExtractZLib(ContiguousByteBufferView Input)
{
	return Extract<&libdeflate_zlib_decompress>(Input);
}





Compression::Result Compression::Extractor::ExtractZLib(ContiguousByteBufferView Input, size_t UncompressedSize)
{
	return Extract<&libdeflate_zlib_decompress>(Input, UncompressedSize);
}





template <auto Algorithm>
Compression::Result Compression::Extractor::Extract(const ContiguousByteBufferView Input)
{
	// Allocate space on the heap to write the compression result, increasing in powers of 2.

	auto DynamicCapacity = Result::StaticCapacity * 2;
	while (true)
	{
		size_t BytesWrittenOut {};
		auto Dynamic = cpp20::make_unique_for_overwrite<Result::Dynamic::element_type[]>(DynamicCapacity);

		switch (Algorithm(m_Handle, Input.data(), Input.size(), Dynamic.get(), DynamicCapacity, &BytesWrittenOut))
		{
			case libdeflate_result::LIBDEFLATE_SUCCESS: return { std::move(Dynamic), BytesWrittenOut };
			case libdeflate_result::LIBDEFLATE_INSUFFICIENT_SPACE:
			{
				DynamicCapacity *= 2;
				continue;
			}
			default: throw std::runtime_error("Data extraction failed.");
		}
	}
}





template <auto Algorithm>
Compression::Result Compression::Extractor::Extract(const ContiguousByteBufferView Input, size_t UncompressedSize)
{
	if (
		auto Dynamic = cpp20::make_unique_for_overwrite<Result::Dynamic::element_type[]>(UncompressedSize);
		Algorithm(m_Handle, Input.data(), Input.size(), Dynamic.get(), UncompressedSize, nullptr) == libdeflate_result::LIBDEFLATE_SUCCESS
	)
	{

		return { std::move(Dynamic), UncompressedSize };

	}


	throw std::runtime_error("Data extraction failed.");
}
