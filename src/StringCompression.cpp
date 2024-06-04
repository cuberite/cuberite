
// StringCompression.cpp

// Implements the wrapping functions for compression and decompression

#include "Globals.h"
#include "ByteBuffer.h"
#include "StringCompression.h"

#include <libdeflate.h>





std::string_view Compression::Result::GetStringView() const
{
	/* Use the GetView method to obtain a std::basic_string_view <std::byte>.
	Next, cast what is returned by the data() method from a pointer to std::byte, to a pointer to char.
	Also get the size of the array returned by GetView(). */

	const auto View = GetView();
	return { reinterpret_cast<const char *>(View.data()), View.size() };
}





ContiguousByteBufferView Compression::Result::GetView() const
{
	/* Compression::Result::Storage is a smart pointer to a std::vector containing std::bytes.
	Assuming that the current Compression::Result::Storage attribute is populated, the vector's front() method gets the first element in the vector.
	As a pointer is needed, obtain the reference to the first element of Compression::Result::Storage to get a compatible pointer.
	Obtain the size of the array of bytes as this is necessary for std::string_view objects. */

	return
	{
		&(this->Storage.get()->front()),
		Size
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
	/* Allocate space on the heap to write the compression result, increasing in powers of 2.
	This will either succeed, or except with bad_alloc. */

	auto DynamicCapacity = Result::StaticCapacity * 2;
	while (true)
	{
		// Write to a temporary buffer, Dynamic, using the variable DynamicCapacity to allocate memory.
		auto Dynamic = cpp20::make_unique_for_overwrite<std::byte[]>(DynamicCapacity);
		const auto BytesWrittenOut = Algorithm(m_Handle, Input, Size, Dynamic.get(), DynamicCapacity);
		if (BytesWrittenOut != 0)
		{
			/* Transfer the contents of Dynamic to the Compression::Result::Storage vector attribute.
			Also use DynamicCapacity for the Compression::Result::Size size_t attribute. */
			return
			{
				std::make_unique <std::vector <std::byte> > (Dynamic.get(), (Dynamic.get() + DynamicCapacity)),
				DynamicCapacity
			};
		}
		DynamicCapacity *= 2;
	}
}





/* Compression functions, which are public and call the Compression::Compressor::Compress method, which is private.
The relevant libdeflate function is passed through as a template argument. */

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





/* Extract functions, which are public and call the Compression::Extractor::Extract method, which is private.
The relevant libdeflate function is passed through as a template argument. */

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
		// Initialize BytesWrittenOut to 0 - MSVC may complain otherwise.
		size_t BytesWrittenOut = 0;
		auto Dynamic = cpp20::make_unique_for_overwrite<std::byte[]>(DynamicCapacity);

		switch (Algorithm(m_Handle, Input.data(), Input.size(), Dynamic.get(), DynamicCapacity, &BytesWrittenOut))
		{

			case libdeflate_result::LIBDEFLATE_SUCCESS:

				/* Transfer the contents of Dynamic to the Compression::Result::Storage vector attribute.
				Also use DynamicCapacity for the Compression::Result::Size size_t attribute. */

				return
				{
					std::make_unique <std::vector <std::byte> > (Dynamic.get(), (Dynamic.get() + DynamicCapacity)),
					DynamicCapacity
				};

			case libdeflate_result::LIBDEFLATE_INSUFFICIENT_SPACE:
			{
				// Double the memory buffer size to allocate and iterate in the loop.
				DynamicCapacity *= 2;
				continue;
			}

			default:
			{
				throw std::runtime_error("Data extraction failed.");
			}

		}
	}
}





template <auto Algorithm>
Compression::Result Compression::Extractor::Extract(const ContiguousByteBufferView Input, size_t UncompressedSize)
{
	/* Initialize the variable Dynamic within the condtional statement.
	A separate parameter, UncompressedSize, is used instead of the default DynamicCapacity variable in order to allocate the size of memory for the vector.
	Should the libdeflate command not return LIBDEFLATE_SUCCESS, throw a runtime error. */

	if (auto Dynamic = cpp20::make_unique_for_overwrite<std::byte[]>(UncompressedSize);
		Algorithm(m_Handle, Input.data(), Input.size(), Dynamic.get(), UncompressedSize, nullptr) == libdeflate_result::LIBDEFLATE_SUCCESS
	)
	{
		return
		{
			/* Use the pointer to Dynamic and get the pointer of the index after the last element in Dynamic.
			Compression::Result::Storage is then initialized using the iterators through the array's elements.
			Also return the size of the data, which is copied from UncompressedSize. */

			std::make_unique <std::vector <std::byte> > (Dynamic.get(), (Dynamic.get() + UncompressedSize)),
			UncompressedSize
		};
	}

	throw std::runtime_error("Data extraction failed.");
}

