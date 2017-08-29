// UUID.h

// Declares the cUUID class representing a Universally Unique Identifier

#pragma once


// tolua_begin

class cUUID
{
public:
	/** Default constructed "nil" UUID */
	cUUID():
		m_UUID()
	{
	}

	/** Lexicographically compare bytes with another UUID.
	Returns:
		0 when equal to a_Other,
		< 0 when less than a_Other,
		> 0 when greater than a_Other */
	int Compare(const cUUID & a_Other) const
	{
		return std::memcmp(m_UUID.data(), a_Other.m_UUID.data(), m_UUID.size());
	}

	/** Returns true if this contains the "nil" UUID with all bits set to 0 */
	bool IsNil() const
	{
		return (m_UUID == std::array<Byte, 16>{{0}});
	}

	/** Tries to interpret the string as a short or long form UUID and assign from it.
	On error, returns false and does not set the value. */
	bool FromString(const AString & a_StringUUID);

	/** Converts the UUID to a short form string (i.e without dashes). */
	AString ToShortString() const;

	/** Converts the UUID to a long form string (i.e. with dashes). */
	AString ToLongString() const;

	/** Returns the version number of the UUID. */
	UInt8 Version() const;

	/** Returns the variant number of the UUID. */
	UInt8 Variant() const;

	/** Generates a version 3, variant 1 UUID based on the md5 hash of a_Name. */
	static cUUID GenerateVersion3(const AString & a_Name);

	// tolua_end

	/** Converts UUID to raw memory representation, respecting UUID variant. */
	std::array<Byte, 16> ToRaw() const;

	/** Assigns from raw memory representation, respecting UUID variant. */
	void FromRaw(const std::array<Byte, 16> & a_Raw);

private:
	/** Binary UUID stored big-endian. */
	std::array<Byte, 16> m_UUID;
};  // tolua_export


// Comparison operators:

inline bool operator == (const cUUID & a_Lhs, const cUUID & a_Rhs)
{
	return (a_Lhs.Compare(a_Rhs) == 0);
}

inline bool operator != (const cUUID & a_Lhs, const cUUID & a_Rhs)
{
	return (a_Lhs.Compare(a_Rhs) != 0);
}

inline bool operator < (const cUUID & a_Lhs, const cUUID & a_Rhs)
{
	return (a_Lhs.Compare(a_Rhs) < 0);
}

inline bool operator <= (const cUUID & a_Lhs, const cUUID & a_Rhs)
{
	return (a_Lhs.Compare(a_Rhs) <= 0);
}

inline bool operator > (const cUUID & a_Lhs, const cUUID & a_Rhs)
{
	return (a_Lhs.Compare(a_Rhs) > 0);
}

inline bool operator >= (const cUUID & a_Lhs, const cUUID & a_Rhs)
{
	return (a_Lhs.Compare(a_Rhs) >= 0);
}


