
#pragma once

#include <variant>

#include "NBTDef.h"
/*
This class represents a mutable NBT tree. Everything is stored in a variant.
*/

// fwd:
class cParsedNBT;
class cFastNBTWriter;

// fwd:
class cNBTContent;
class cNBT;
namespace NBT
{
	using cByteArray = std::vector<char>;  // Todo: Make this a Int8
	using cIntArray = std::vector<Int32>;
	using cList = std::vector<cNBT>;
	using cCompound = std::unordered_map<AString, std::pair<eTagType, cNBT>, std::hash<AString>>;
	class cEmptyTag{};

	// The oder must match the eTagType enum
	using cNBTVariant = std::variant<
		cEmptyTag,   // TAG_End
		Int8,        // TAG_Byte
		Int16,       // TAG_Short
		Int32,       // TAG_Int
		Int64,       // TAG_Long
		float,       // TAG_Float
		double,      // TAG_Double
		AString,     // TAG_String
		cCompound,   // TAG_Compound
		cList,       // TAG_List
		cByteArray,  // TAG_ByteArray
		cIntArray    // TAG_IntArray
	>;

	// We need this wrapper to be able to have the circular behaviour in the variant
	class cNBTContent :
			public virtual cNBTVariant
	{
	public:
		cNBTContent() : cNBTVariant() {}
		cNBTContent(cNBTVariant a_Content) : cNBTVariant(std::move(a_Content)) {}
		using cNBTVariant::operator=;
	};
}

class cNBT
{
public:
	/** Creates an empty NBT tag. TAG_End. */
	cNBT();
	cNBT(NBT::cNBTContent a_Content);
	cNBT(NBT::cNBTVariant a_Content);

	/** Returns a reference to the stored value. */
	const NBT::cNBTContent & Expose() const;

	/** If there is a list stored you may use this to push back a element */
	void Push(NBT::cNBTContent a_NewContent);

	/** Returns a String containing the tag in the notchian way. */
	static void Serialize(const NBT::cCompound & a_Compound, cFastNBTWriter & a_Writer);

	static NBT::cCompound Deserialize(const cParsedNBT & a_NBT, int a_TagIdx);

private:
	NBT::cNBTContent m_Content;

	static void SerializeEntry(const AString & a_Name, const cNBT & a_Entry, eTagType a_TagType, cFastNBTWriter & a_Writer);
	static NBT::cNBTVariant DeserializeEntry(const cParsedNBT & a_NBT, int a_TagIdx, eTagType a_TagType);
};
