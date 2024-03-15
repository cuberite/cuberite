
// StringCompression.h

// Interfaces to the wrapping functions for compression and decompression

#pragma once





struct libdeflate_compressor;
struct libdeflate_decompressor;





namespace Compression
{
	/** Contains the result of a compression or extraction operation. */
	struct Result
	{
		/** Static is set to be 128 times 1024 bytes.
		This is used as a placeholder in order to calculate the target memory to be allocated on the heap. */
		using Static = std::array<std::byte, 128 KiB>;

		/** Used in StringCompression.cpp to calculate DynamicCompresion.
		This is a variable that is used to calculate the memory to allocate on the heap. */
		static constexpr size_t StaticCapacity = sizeof(Compression::Result::Static) / sizeof(Compression::Result::Static::value_type);

		/** Returns a view (of type char) of the internal store. */
		std::string_view GetStringView() const;

		/** Returns a view (of type std::byte) of the internal store. */
		ContiguousByteBufferView GetView() const;

		/** A store allocated on the heap.
		This uses a smart pointer. */
		std::unique_ptr <std::vector <std::byte>> Storage;

		/** The length of valid data in the store.
		One can always use the vector's size() method, but a separate Size attribute is much more readable.
		The size of the memory allocated is needed for the data type conversions to std::string_view <std::byte> and std::basic_string_view <std::byte>. */
		size_t Size;
	};

	/** Contains routines for data compression. */
	class Compressor
	{
	public:

		/** Creates a new compressor instance with a compression factor [0-12]. */
		Compressor(int CompressionFactor = 6);

		/** Destructor for the the compressor instance. */
		~Compressor();

		/** Public methods for different compression options. */
		Result CompressGZip(ContiguousByteBufferView Input);
		Result CompressZLib(ContiguousByteBufferView Input);
		Result CompressZLib(const void * Input, size_t Size);

	private:

		/** Compress method.
		Uses a libdeflate function address as a template argument for Algorithm. */
		template <auto Algorithm>
		Result Compress(const void * Input, size_t Size);

		libdeflate_compressor * m_Handle;
	};

	/** Contains routines for data extraction. */
	class Extractor
	{
	public:

		/** Creates a new extractor instance. */
		Extractor();

		/** Destructor for the extractor instance. */
		~Extractor();

		/** Public methods for different extraction options. */
		Result ExtractGZip(ContiguousByteBufferView Input);
		Result ExtractZLib(ContiguousByteBufferView Input);
		Result ExtractZLib(ContiguousByteBufferView Input, size_t UncompressedSize);

	private:

		/** Extract method.
		Uses a libdeflate function address as a template argument for Algorithm. */
		template <auto Algorithm> Result Extract(ContiguousByteBufferView Input);
		template <auto Algorithm> Result Extract(ContiguousByteBufferView Input, size_t UncompressedSize);

		libdeflate_decompressor * m_Handle;
	};
}

