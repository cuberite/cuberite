#pragma once

#include <array>
template <typename T>
using Bytes = std::array<std::byte, sizeof(T)>;

// bit_cast used for going between ulong, float, etc. It's a new C++20 feature
#ifdef __cpp_lib_bit_cast
#include <bit>
using std::bit_cast;

// Fallback in case we're using C++17
#else
// bit-for-bit convert one type to another. In C++ the only non-UB way to do this is *memcpy*. Nearly every other
// option is a strict aliasing violation.
template<class To, class From>
std::enable_if_t<
	sizeof(To) == sizeof(From),
	To>
bit_cast(const From &src) noexcept
{
	To dst;
	std::memcpy(&dst, &src, sizeof(To));
	return dst;
}
#endif


/** Converts a 16-bit host integer or float value to bytes in big-endian (Network) order.
@tparam Value The host 16-bit type (Int16, UInt16). Usually inferred.
@param a_Value The input integer or float value.
@return The resulting bytes. */
template<typename Value, std::enable_if_t<sizeof(Value) == 2, bool> = true>
inline Bytes<Value> HostToNetwork(Value a_Value)
{
	UInt16 Bits = bit_cast<UInt16>(a_Value);
	return
	{
		std::byte(Bits >> 8),
		std::byte(Bits)
	};
}

/** Converts a 32-bit host integer or float value to bytes in big-endian (Network) order.
@tparam Value The host 32-bit type (Int32, UInt32, float). Usually inferred.
@param a_Value The input integer or float value.
@return The resulting bytes. */
template<typename Value, std::enable_if_t<sizeof(Value) == 4, bool> = true>
inline Bytes<Value> HostToNetwork(Value a_Value)
{
	UInt32 Bits = bit_cast<UInt32>(a_Value);
	return
	{
		std::byte(Bits >> 24),
		std::byte(Bits >> 16),
		std::byte(Bits >> 8),
		std::byte(Bits)
	};
}

/** Converts a 64-bit host integer or float value to bytes in big-endian (Network) order.
@tparam Value The host 64-bit type (Int64, UInt64, double). Usually inferred.
@param a_Value The input integer or float value.
@return The resulting bytes. */
template<typename Value, std::enable_if_t<sizeof(Value) == 8, bool> = true>
inline Bytes<Value> HostToNetwork(Value a_Value)
{
	UInt64 Bits = bit_cast<UInt64>(a_Value);
	return
	{
		std::byte(Bits >> 56),
		std::byte(Bits >> 48),
		std::byte(Bits >> 40),
		std::byte(Bits >> 32),
		std::byte(Bits >> 24),
		std::byte(Bits >> 16),
		std::byte(Bits >> 8),
		std::byte(Bits)
	};
}

/** Reads a 16-bit integer or float value from big-endian (Network) bytes.
@tparam Value The desired 16-bit type (Int16, UInt16)
@param a_Value The input bytes.
@return The resulting integer or float value. */
template<typename Value, std::enable_if_t<sizeof(Value) == 2, bool> = true>
inline Value NetworkToHost(Bytes<Value> a_Value)
{
	UInt16 val = UInt16(
		UInt16(a_Value[0]) << 8 |
		UInt16(a_Value[1]));
	return bit_cast<Value>(val);
}

/** Reads a 32-bit integer or float value from big-endian (Network) bytes.
@tparam Value The desired 32-bit type (Int32, UInt32, float)
@param a_Value The input bytes.
@return The resulting integer or float value. */
template<typename Value, std::enable_if_t<sizeof(Value) == 4, bool> = true>
inline Value NetworkToHost(Bytes<Value> a_Value)
{
	UInt32 val = UInt32(
		UInt32(a_Value[0]) << 24 |
		UInt32(a_Value[1]) << 16 |
		UInt32(a_Value[2]) << 8 |
		UInt32(a_Value[3]));
	return bit_cast<Value>(val);
}

/** Reads a 64-bit integer or float value from big-endian (Network) bytes.
@tparam Value The desired 64-bit type (Int64, UInt64, double)
@param a_Value The input bytes.
@return The resulting integer or float value. */
template<typename Value, std::enable_if_t<sizeof(Value) == 8, bool> = true>
inline Value NetworkToHost(Bytes<Value> a_Value)
{
	UInt64 val = UInt64(
		UInt64(a_Value[0]) << 56 |
		UInt64(a_Value[1]) << 48 |
		UInt64(a_Value[2]) << 40 |
		UInt64(a_Value[3]) << 32 |
		UInt64(a_Value[4]) << 24 |
		UInt64(a_Value[5]) << 16 |
		UInt64(a_Value[6]) << 8 |
		UInt64(a_Value[7]));
	return bit_cast<Value>(val);
}

/** Reads an integer or float type from its big-endian (Network) bytes.
@tparam Value The desired result type (Int16 / 32 / 64, UInt16 / 32 / 64, float, double).
@param a_Mem A pointer to the first input byte. Length is inferred from the result type.
@return The resulting integer or float value.

Consider using NetworkToHost when the data is owned since it provides additional type safety (length is known). */
template<typename Value>
inline Value NetworkBufToHost(const std::byte* a_Mem)
{
	// Copy unfortunately needed to add the length information required by the rest of the API.
	// Gets completely optimised out in my testing.
	Bytes<Value> bytes;
	std::copy(a_Mem, a_Mem + sizeof(Value), bytes.begin());
	return NetworkToHost<Value>(bytes);
}
