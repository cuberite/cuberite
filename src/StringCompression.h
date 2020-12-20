
// StringCompression.h

// Interfaces to the wrapping functions for compression and decompression

#pragma once





class cByteBuffer;

struct libdeflate_compressor;
struct libdeflate_decompressor;





namespace Compression
{
	struct Result
	{
		using Static = std::array<std::byte, 128 KiB>;
		using Dynamic = std::unique_ptr<std::byte[]>;

		static constexpr size_t StaticCapacity = sizeof(Compression::Result::Static) / sizeof(Compression::Result::Static::value_type);

		std::string_view GetStringView() const;
		ContiguousByteBufferView GetView() const;

		std::variant<Static, Dynamic> Storage;
		size_t Size;
	};

	class Compressor
	{
	public:

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

	class Extractor
	{
	public:

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
