
// FastNBT.h

// Interfaces to the fast NBT parser and writer

/*
The fast parser parses the data into a vector of cFastNBTTag structures. These structures describe the NBT tree,
but themselves are allocated in a vector, thus minimizing reallocation.
The structures have a minimal constructor, setting all member "pointers" to "invalid".

The fast writer doesn't need a NBT tree structure built beforehand, it is commanded to open, append and close tags
(just like XML); it keeps the internal tag stack and reports errors in usage.
It directly outputs a string containing the serialized NBT data.
*/





#pragma once

#include <system_error>
#include "../Endianness.h"





enum eTagType
{
	TAG_Min       = 0,  // The minimum value for a tag type
	TAG_End       = 0,
	TAG_Byte      = 1,
	TAG_Short     = 2,
	TAG_Int       = 3,
	TAG_Long      = 4,
	TAG_Float     = 5,
	TAG_Double    = 6,
	TAG_ByteArray = 7,
	TAG_String    = 8,
	TAG_List      = 9,
	TAG_Compound  = 10,
	TAG_IntArray  = 11,
	TAG_Max       = 11,  // The maximum value for a tag type
} ;





/** This structure is used for all NBT tags.
It contains indices to the parent array of tags, building the NBT tree this way.
Also contains indices into the data stream being parsed, used for values;
NO dynamically allocated memory is used!
Structure (all with the tree structure it describes) supports moving in memory (std::vector reallocation)
*/
struct cFastNBTTag
{
public:

	eTagType m_Type;

	// The following members are indices into the data stream. m_DataLength == 0 if no data available
	// They must not be pointers, because the datastream may be copied into another AString object in the meantime.
	size_t m_NameStart;
	size_t m_NameLength;
	size_t m_DataStart;
	size_t m_DataLength;

	// The following members are indices into the array returned; -1 if not valid
	// They must not be pointers, because pointers would not survive std::vector reallocation
	int m_Parent;
	int m_PrevSibling;
	int m_NextSibling;
	int m_FirstChild;
	int m_LastChild;

	cFastNBTTag(eTagType a_Type, int a_Parent) :
		m_Type(a_Type),
		m_NameStart(0),
		m_NameLength(0),
		m_DataStart(0),
		m_DataLength(0),
		m_Parent(a_Parent),
		m_PrevSibling(-1),
		m_NextSibling(-1),
		m_FirstChild(-1),
		m_LastChild(-1)
	{
	}

	cFastNBTTag(eTagType a_Type, int a_Parent, int a_PrevSibling) :
		m_Type(a_Type),
		m_NameStart(0),
		m_NameLength(0),
		m_DataStart(0),
		m_DataLength(0),
		m_Parent(a_Parent),
		m_PrevSibling(a_PrevSibling),
		m_NextSibling(-1),
		m_FirstChild(-1),
		m_LastChild(-1)
	{
	}
} ;





enum class eNBTParseError
{
	npSuccess = 0,
	npNeedBytes,
	npNoTopLevelCompound,
	npCompoundImbalancedTag,
	npStringMissingLength,
	npStringInvalidLength,
	npListMissingType,
	npListMissingLength,
	npListInvalidLength,
	npSimpleMissing,
	npArrayMissingLength,
	npArrayInvalidLength,
	npUnknownTag,
};

// The following is required to make an error_code constructible from an eNBTParseError
std::error_code make_error_code(eNBTParseError a_Err) NOEXCEPT;

namespace std
{
	template <>
	struct is_error_code_enum<eNBTParseError>:
		public std::true_type
	{
	};
}





/** Parses and contains the parsed data
Also implements data accessor functions for tree traversal and value getters
The data pointer passed in the constructor is assumed to be valid throughout the object's life. Care must be taken not to initialize from a temporary.
The parser decomposes the input data into a tree of tags that is stored as an array of cFastNBTTag items,
and accessing the tree is done by using the array indices for tags. Each tag stores the indices for its parent,
first child, last child, prev sibling and next sibling, a value of -1 indicates that the indice is not valid.
Each primitive tag also stores the length of the contained data, in bytes.
*/
class cParsedNBT
{
public:
	cParsedNBT(const char * a_Data, size_t a_Length);

	bool IsValid(void) const { return (m_Error == eNBTParseError::npSuccess); }

	/** Returns the error code for the parsing of the NBT data. */
	std::error_code GetErrorCode() const { return m_Error; }

	/** Returns the position where an error occurred while parsing. */
	size_t GetErrorPos() const { return m_Pos; }

	/** Returns the root tag of the hierarchy. */
	int GetRoot(void) const { return 0; }

	/** Returns the first child of the specified tag, or -1 if none / not applicable. */
	int GetFirstChild (int a_Tag) const { return m_Tags[static_cast<size_t>(a_Tag)].m_FirstChild; }

	/** Returns the last child of the specified tag, or -1 if none / not applicable. */
	int GetLastChild  (int a_Tag) const { return m_Tags[static_cast<size_t>(a_Tag)].m_LastChild; }

	/** Returns the next sibling of the specified tag, or -1 if none. */
	int GetNextSibling(int a_Tag) const { return m_Tags[static_cast<size_t>(a_Tag)].m_NextSibling; }

	/** Returns the previous sibling of the specified tag, or -1 if none. */
	int GetPrevSibling(int a_Tag) const { return m_Tags[static_cast<size_t>(a_Tag)].m_PrevSibling; }

	/** Returns the length of the tag's data, in bytes.
	Not valid for Compound or List tags! */
	size_t GetDataLength (int a_Tag) const
	{
		ASSERT(m_Tags[static_cast<size_t>(a_Tag)].m_Type != TAG_List);
		ASSERT(m_Tags[static_cast<size_t>(a_Tag)].m_Type != TAG_Compound);
		return m_Tags[static_cast<size_t>(a_Tag)].m_DataLength;
	}

	/** Returns the data stored in this tag.
	Not valid for Compound or List tags! */
	const char * GetData(int a_Tag) const
	{
		ASSERT(m_Tags[static_cast<size_t>(a_Tag)].m_Type != TAG_List);
		ASSERT(m_Tags[static_cast<size_t>(a_Tag)].m_Type != TAG_Compound);
		return m_Data + m_Tags[static_cast<size_t>(a_Tag)].m_DataStart;
	}

	/** Returns the direct child tag of the specified name, or -1 if no such tag. */
	int FindChildByName(int a_Tag, const AString & a_Name) const
	{
		return FindChildByName(a_Tag, a_Name.c_str(), a_Name.length());
	}

	/** Returns the direct child tag of the specified name, or -1 if no such tag. */
	int FindChildByName(int a_Tag, const char * a_Name, size_t a_NameLength = 0) const;

	/** Returns the child tag of the specified path (Name1 / Name2 / Name3...), or -1 if no such tag. */
	int FindTagByPath(int a_Tag, const AString & a_Path) const;

	eTagType GetType(int a_Tag) const { return m_Tags[static_cast<size_t>(a_Tag)].m_Type; }

	/** Returns the children type for a List tag; undefined on other tags. If list empty, returns TAG_End. */
	eTagType GetChildrenType(int a_Tag) const
	{
		ASSERT(m_Tags[static_cast<size_t>(a_Tag)].m_Type == TAG_List);
		return (m_Tags[static_cast<size_t>(a_Tag)].m_FirstChild < 0) ? TAG_End : m_Tags[static_cast<size_t>(m_Tags[static_cast<size_t>(a_Tag)].m_FirstChild)].m_Type;
	}

	/** Returns the value stored in a Byte tag. Not valid for any other tag type. */
	inline unsigned char GetByte(int a_Tag) const
	{
		ASSERT(m_Tags[static_cast<size_t>(a_Tag)].m_Type == TAG_Byte);
		return static_cast<unsigned char>(m_Data[static_cast<size_t>(m_Tags[static_cast<size_t>(a_Tag)].m_DataStart)]);
	}

	/** Returns the value stored in a Short tag. Not valid for any other tag type. */
	inline Int16 GetShort(int a_Tag) const
	{
		ASSERT(m_Tags[static_cast<size_t>(a_Tag)].m_Type == TAG_Short);
		return GetBEShort(m_Data + m_Tags[static_cast<size_t>(a_Tag)].m_DataStart);
	}

	/** Returns the value stored in an Int tag. Not valid for any other tag type. */
	inline Int32 GetInt(int a_Tag) const
	{
		ASSERT(m_Tags[static_cast<size_t>(a_Tag)].m_Type == TAG_Int);
		return GetBEInt(m_Data + m_Tags[static_cast<size_t>(a_Tag)].m_DataStart);
	}

	/** Returns the value stored in a Long tag. Not valid for any other tag type. */
	inline Int64 GetLong(int a_Tag) const
	{
		ASSERT(m_Tags[static_cast<size_t>(a_Tag)].m_Type == TAG_Long);
		return NetworkToHostLong8(m_Data + m_Tags[static_cast<size_t>(a_Tag)].m_DataStart);
	}

	/** Returns the value stored in a Float tag. Not valid for any other tag type. */
	inline float GetFloat(int a_Tag) const
	{
		ASSERT(m_Tags[static_cast<size_t>(a_Tag)].m_Type == TAG_Float);

		// Cause a compile-time error if sizeof(float) != 4
		// If your platform produces a compiler error here, you'll need to add code that manually decodes 32-bit floats
		char Check1[5 - sizeof(float)];  // Fails if sizeof(float) > 4
		char Check2[sizeof(float) - 3];  // Fails if sizeof(float) < 4
		UNUSED_VAR(Check1);
		UNUSED_VAR(Check2);

		Int32 i = GetBEInt(m_Data + m_Tags[static_cast<size_t>(a_Tag)].m_DataStart);
		float f;
		memcpy(&f, &i, sizeof(f));
		return f;
	}

	/** Returns the value stored in a Double tag. Not valid for any other tag type. */
	inline double GetDouble(int a_Tag) const
	{
		// Cause a compile-time error if sizeof(double) != 8
		// If your platform produces a compiler error here, you'll need to add code that manually decodes 64-bit doubles
		char Check1[9 - sizeof(double)];  // Fails if sizeof(double) > 8
		char Check2[sizeof(double) - 7];  // Fails if sizeof(double) < 8
		UNUSED_VAR(Check1);
		UNUSED_VAR(Check2);

		ASSERT(m_Tags[static_cast<size_t>(a_Tag)].m_Type == TAG_Double);
		return NetworkToHostDouble8(m_Data + m_Tags[static_cast<size_t>(a_Tag)].m_DataStart);
	}

	/** Returns the value stored in a String tag. Not valid for any other tag type. */
	inline AString GetString(int a_Tag) const
	{
		ASSERT(m_Tags[static_cast<size_t>(a_Tag)].m_Type == TAG_String);
		AString res;
		res.assign(m_Data + m_Tags[static_cast<size_t>(a_Tag)].m_DataStart, static_cast<size_t>(m_Tags[static_cast<size_t>(a_Tag)].m_DataLength));
		return res;
	}

	/** Returns the tag's name. For tags that are not named, returns an empty string. */
	inline AString GetName(int a_Tag) const
	{
		AString res;
		res.assign(m_Data + m_Tags[static_cast<size_t>(a_Tag)].m_NameStart, static_cast<size_t>(m_Tags[static_cast<size_t>(a_Tag)].m_NameLength));
		return res;
	}

protected:
	const char *             m_Data;
	size_t                   m_Length;
	std::vector<cFastNBTTag> m_Tags;
	eNBTParseError           m_Error;  // npSuccess if parsing succeeded

	// Used while parsing:
	size_t m_Pos;

	eNBTParseError Parse(void);
	eNBTParseError ReadString(size_t & a_StringStart, size_t & a_StringLen);  // Reads a simple string (2 bytes length + data), sets the string descriptors
	eNBTParseError ReadCompound(void);  // Reads the latest tag as a compound
	eNBTParseError ReadList(eTagType a_ChildrenType);  // Reads the latest tag as a list of items of type a_ChildrenType
	eNBTParseError ReadTag(void);       // Reads the latest tag, depending on its m_Type setting
} ;





class cFastNBTWriter
{
public:
	cFastNBTWriter(const AString & a_RootTagName = "");

	void BeginCompound(const AString & a_Name);
	void EndCompound(void);

	void BeginList(const AString & a_Name, eTagType a_ChildrenType);
	void EndList(void);

	void AddByte     (const AString & a_Name, unsigned char a_Value);
	void AddShort    (const AString & a_Name, Int16 a_Value);
	void AddInt      (const AString & a_Name, Int32 a_Value);
	void AddLong     (const AString & a_Name, Int64 a_Value);
	void AddFloat    (const AString & a_Name, float a_Value);
	void AddDouble   (const AString & a_Name, double a_Value);
	void AddString   (const AString & a_Name, const AString & a_Value);
	void AddByteArray(const AString & a_Name, const char * a_Value, size_t a_NumElements);
	void AddIntArray (const AString & a_Name, const int *  a_Value, size_t a_NumElements);

	void AddByteArray(const AString & a_Name, const AString & a_Value)
	{
		AddByteArray(a_Name, a_Value.data(), a_Value.size());
	}

	const AString & GetResult(void) const {return m_Result; }

	void Finish(void);

protected:

	struct sParent
	{
		int m_Type;   // TAG_Compound or TAG_List
		int m_Pos;    // for TAG_List, the position of the list count
		int m_Count;  // for TAG_List, the element count
		eTagType m_ItemType;  // for TAG_List, the element type
	} ;

	static const int MAX_STACK = 50;  // Highly doubtful that an NBT would be constructed this many levels deep

	// These two fields emulate a stack. A raw array is used due to speed issues - no reallocations are allowed.
	sParent m_Stack[MAX_STACK];
	int     m_CurrentStack;

	AString m_Result;

	bool IsStackTopCompound(void) const { return (m_Stack[m_CurrentStack].m_Type == TAG_Compound); }

	void WriteString(const char * a_Data, UInt16 a_Length);

	inline void TagCommon(const AString & a_Name, eTagType a_Type)
	{
		// If we're directly inside a list, check that the list is of the correct type:
		ASSERT((m_Stack[m_CurrentStack].m_Type != TAG_List) || (m_Stack[m_CurrentStack].m_ItemType == a_Type));

		if (IsStackTopCompound())
		{
			// Compound: add the type and name:
			m_Result.push_back(static_cast<char>(a_Type));
			WriteString(a_Name.c_str(), static_cast<UInt16>(a_Name.length()));
		}
		else
		{
			// List: add to the counter
			m_Stack[m_CurrentStack].m_Count++;
		}
	}
} ;




