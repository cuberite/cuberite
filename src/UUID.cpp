// UUID.h

// Defines the cUUID class representing a Universally Unique Identifier

#include "Globals.h"
#include "UUID.h"

#include "mbedtls/md5.h"


/** UUID normalised in textual form. */
struct sShortUUID
{
	char Data[32]{};
	bool IsValid = false;
};

/** Returns the given UUID in shortened form with IsValid indicating success.
Doesn't check digits are hexadecimal but does check dashes if long form. */
static sShortUUID ShortenUUID(const AString & a_StringUUID)
{
	sShortUUID UUID;
	switch (a_StringUUID.size())
	{
		case 32:
		{
			// Already a short UUID
			std::memcpy(UUID.Data, a_StringUUID.data(), 32);
			UUID.IsValid = true;
			break;
		}
		case 36:
		{
			// Long UUID, confirm dashed
			if (
				(a_StringUUID[ 8] != '-') ||
				(a_StringUUID[13] != '-') ||
				(a_StringUUID[18] != '-') ||
				(a_StringUUID[23] != '-')
			)
			{
				break;
			}

			// Copy everying but the dashes from the string
			std::memcpy(UUID.Data,      a_StringUUID.data(),       8);
			std::memcpy(UUID.Data +  8, a_StringUUID.data() +  9,  4);
			std::memcpy(UUID.Data + 12, a_StringUUID.data() + 14,  4);
			std::memcpy(UUID.Data + 16, a_StringUUID.data() + 19,  4);
			std::memcpy(UUID.Data + 20, a_StringUUID.data() + 24, 12);
			UUID.IsValid = true;
		}
		default: break;
	}
	return UUID;
}





/** Returns the integer value of the hex digit or 0xff if invalid. */
static Byte FromHexDigit(char a_Hex)
{
	if (('0' <= a_Hex) && (a_Hex <= '9'))
	{
		return static_cast<Byte>(a_Hex - '0');
	}
	if (('a' <= a_Hex) && (a_Hex <= 'f'))
	{
		return static_cast<Byte>(10 + (a_Hex - 'a'));
	}
	if (('A' <= a_Hex) && (a_Hex <= 'F'))
	{
		return static_cast<Byte>(10 + (a_Hex - 'A'));
	}
	return 0xff;
}





/** From a number in the range [0, 16), returns the corresponding hex digit in lowercase. */
static char ToHexDigit(UInt8 a_Nibble)
{
	ASSERT((a_Nibble & 0xf0) == 0);
	return static_cast<char>(
		(a_Nibble < 10) ?
		('0' + a_Nibble) :
		('a' + (a_Nibble - 10))
	);
}





////////////////////////////////////////////////////////////////////////////////
// cUUID:

bool cUUID::FromString(const AString & a_StringUUID)
{
	sShortUUID Norm = ShortenUUID(a_StringUUID);
	if (!Norm.IsValid)
	{
		return false;
	}

	std::array<Byte, 16> ParsedUUID{{0}};
	for (size_t i = 0; i != m_UUID.size(); ++i)
	{
		Byte HighNibble = FromHexDigit(Norm.Data[2 * i    ]);
		Byte LowNibble  = FromHexDigit(Norm.Data[2 * i + 1]);
		if ((HighNibble > 0x0f) || (LowNibble > 0x0f))
		{
			// Invalid hex digit
			return false;
		}

		ParsedUUID[i] = static_cast<Byte>((HighNibble << 4) | LowNibble);
	}

	// Parsed successfully
	m_UUID = ParsedUUID;
	return true;
}





AString cUUID::ToShortString() const
{
	AString ShortString(32, '\0');
	for (size_t i = 0; i != m_UUID.size(); ++i)
	{
		Byte HighNibble = (m_UUID[i] >> 4) & 0x0f;
		Byte LowNibble  = m_UUID[i] & 0x0f;

		ShortString[2 * i    ] = ToHexDigit(HighNibble);
		ShortString[2 * i + 1] = ToHexDigit(LowNibble);
	}
	return ShortString;
}





AString cUUID::ToLongString() const
{
	AString LongString = ToShortString();
	LongString.reserve(36);

	// Convert to long form by inserting the dashes
	auto First = LongString.begin();
	LongString.insert(First +  8, '-');
	LongString.insert(First + 13, '-');
	LongString.insert(First + 18, '-');
	LongString.insert(First + 23, '-');

	return LongString;
}





UInt8 cUUID::Version() const
{
	return static_cast<UInt8>((m_UUID[6] >> 4) & 0x0f);
}





UInt8 cUUID::Variant() const
{
	const Byte VariantBits = static_cast<Byte>((m_UUID[9] >> 5) & 0x07);

	/* Variant bits format:
		bits | variant |      Description
		-----|---------|----------------------
		0xx  |    0    |        Obsolete
		10x  |    1    |     Standard UUID
		110  |    2    | Microsoft Legacy GUID
		111  |    3    |        Reserved
	*/

	if ((VariantBits & 0x04) == 0)
	{
		return 0;
	}
	else if ((VariantBits & 0x02) == 0)
	{
		return 1;
	}
	else if ((VariantBits & 0x01) == 0)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}





std::array<Byte, 16> cUUID::ToRaw() const
{
	std::array<Byte, 16> Raw(m_UUID);
	if (Variant() == 2)
	{
		// Convert to microsoft mixed-endian format
		// First 3 components are host-endian, last 2 are network
		auto First = reinterpret_cast<UInt32 *>(Raw.data());
		*First = ntohl(*First);

		auto Second = reinterpret_cast<UInt16 *>(&Raw[4]);
		*Second = ntohs(*Second);

		auto Third = Second + 1;
		*Third = ntohs(*Third);
	}

	return Raw;
}





void cUUID::FromRaw(const std::array<Byte, 16> & a_Raw)
{
	m_UUID = a_Raw;
	if (Variant() != 2)
	{
		// Standard big-endian formats
		return;
	}

	// Convert from microsoft mixed-endian format
	// First 3 components are host-endian, last 2 are network
	auto First = reinterpret_cast<UInt32 *>(m_UUID.data());
	*First = htonl(*First);

	auto Second = reinterpret_cast<UInt16 *>(&m_UUID[4]);
	*Second = htons(*Second);

	auto Third = Second + 1;
	*Third = htons(*Third);
}





cUUID cUUID::GenerateVersion3(const AString & a_Name)
{
	cUUID UUID;
	// Generate an md5 checksum, and use it as base for the ID:
	const Byte * ByteString = reinterpret_cast<const Byte *>(a_Name.data());
	mbedtls_md5(ByteString, a_Name.length(), UUID.m_UUID.data());

	// Insert version number
	UUID.m_UUID[6] = (UUID.m_UUID[6] & 0x0f) | 0x30;

	/* Insert variant number
		Note that by using 1000 instead of 10xx we are losing 2 bits
		but this is needed for compatibility with the old string uuid generator */
	UUID.m_UUID[8] = (UUID.m_UUID[8] & 0x0f) | 0x80;

	return UUID;
}




