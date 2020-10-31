
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
class cNBT;
namespace NBT
{
	using cByteArray = std::vector<char>;  // Todo: Make this a Int8
	using cIntArray = std::vector<Int32>;
	using cList = std::vector<cNBT>;
	class cCompound;
	class cEmptyTag{};

	// The oder must match the eTagType enum
	using cNBTVariant = std::variant<
		cEmptyTag,    // TAG_End
		Int8,         // TAG_Byte
		Int16,        // TAG_Short
		Int32,        // TAG_Int
		Int64,        // TAG_Long
		float,        // TAG_Float
		double,       // TAG_Double
		AString,      // TAG_String
		cCompound *,  // TAG_Compound
		cList,        // TAG_List
		cByteArray,   // TAG_ByteArray
		cIntArray     // TAG_IntArray
	>;

	// We need this wrapper to be able to have the circular behaviour in the variant
	class cNBTContent :
			public cNBTVariant
	{
	public:
		using cNBTVariant::cNBTVariant;
	};
}

class cNBT
{
public:
	/** Creates a compound tag. TAG_Compound. */
	cNBT();

	/** Creates a new tag from the supplied element.
	Trying to create a Tag from a cList with more then one element will result in a failed assert.
	Please use Push for any operation. */
	cNBT(NBT::cNBTContent a_Content);

	~cNBT();

	cNBT(const cNBT & a_Other) = default;
	cNBT(cNBT && a_Other) = default;
	cNBT & operator = (const cNBT & a_Other) = default;
	cNBT & operator = (cNBT && a_Other) = default;

	/** Returns a reference to the stored value. */
	const NBT::cNBTContent & Expose() const;

	/** If there is a list stored you may use this to push back a element or pop it.
	Pushing the wrong type or pushing when cNBT is not a list will result in a failed assert. */
	void Push(NBT::cNBTContent a_NewContent);
	NBT::cNBTContent Pop(size_t a_Index);
	NBT::cNBTContent Pop(NBT::cList::iterator a_Index);

	/** Writes the cCompound into the cFastNBTWriter. */
	static void Serialize(const NBT::cCompound * a_Compound, cFastNBTWriter & a_Writer);

	/** Writes cParsedNBT into a cCompound, stores it into a cNBT and returns it. */
	static cNBT Deserialize(const cParsedNBT & a_NBT, int a_TagIdx);

private:
	NBT::cNBTContent m_Content;

	static void SerializeEntry(const AString & a_Name, const cNBT & a_Entry, eTagType a_TagType, cFastNBTWriter & a_Writer);
	static NBT::cNBTContent DeserializeEntry(const cParsedNBT & a_NBT, int a_TagIdx, eTagType a_TagType);
};

namespace NBT
{
	class cCompound :
		public std::unordered_map<AString, std::pair<eTagType, cNBT>, std::hash<AString>>
	{
		using Super = std::unordered_map<AString, std::pair<eTagType, cNBT>, std::hash<AString>>;
		using Super::Super;
	};
}
