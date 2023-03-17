
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
		using Static = std::array<std::byte, 128 KiB>;
		using Dynamic = std::unique_ptr<std::byte[]>;

		static constexpr size_t StaticCapacity = sizeof(Compression::Result::Static) / sizeof(Compression::Result::Static::value_type);

		/** Returns a view (of type char) of the internal store. */
		std::string_view GetStringView() const;

		/** Returns a view (of type std::byte) of the internal store. */
		ContiguousByteBufferView GetView() const;

		/** A store allocated on either the stack or heap. */
		std::variant<Static, Dynamic> Storage;

		/** The length of valid data in the store. */
		size_t Size;
	};

	/** Contains routines for data compression. */
	class Compressor
	{
	public:

		/** Creates a new compressor instance with a compression factor [0-12]. */
		Compressor(int CompressionFactor = 6);
		~Compressor();

		Result CompressGZip(ContiguousByteBufferView Input);
		Result CompressZLib(ContiguousByteBufferView Input);
		Result CompressZLib(const void * Input, size_t Size);

	private:

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
		~Extractor();

		Result ExtractGZip(ContiguousByteBufferView Input);
		Result ExtractZLib(ContiguousByteBufferView Input);
		Result ExtractZLib(ContiguousByteBufferView Input, size_t UncompressedSize);

	private:

		template <auto Algorithm> Result Extract(ContiguousByteBufferView Input);
		template <auto Algorithm> Result Extract(ContiguousByteBufferView Input, size_t UncompressedSize);

		libdeflate_decompressor * m_Handle;
	};
}
