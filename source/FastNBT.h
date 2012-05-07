
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

#include "Endianness.h"





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
	int m_NameStart;
	int m_NameLength;
	int m_DataStart;
	int m_DataLength;
	
	// The following members are indices into the array returned; -1 if not valid
	// They must not be pointers, because pointers would not survive std::vector reallocation
	int m_Parent;
	int m_PrevSibling;
	int m_NextSibling;
	int m_FirstChild;
	int m_LastChild;
	
	cFastNBTTag(eTagType a_Type, int a_Parent) :
		m_Type(a_Type),
		m_NameLength(0),
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
		m_NameLength(0),
		m_DataLength(0),
		m_Parent(a_Parent),
		m_PrevSibling(a_PrevSibling),
		m_NextSibling(-1),
		m_FirstChild(-1),
		m_LastChild(-1)
	{
	}
} ;





/** Parses and contains the parsed data
Also implements data accessor functions for tree traversal and value getters
The data pointer passed in the constructor is assumed to be valid throughout the object's life. Care must be taken not to initialize from a temporary.
*/
class cParsedNBT
{
public:
	cParsedNBT(const char * a_Data, int a_Length);
	
	bool IsValid(void) const {return m_IsValid; }
	
	int GetRoot(void) const {return 0; }
	int GetFirstChild (int a_Tag) const { return m_Tags[a_Tag].m_FirstChild; }
	int GetLastChild  (int a_Tag) const { return m_Tags[a_Tag].m_LastChild; }
	int GetNextSibling(int a_Tag) const { return m_Tags[a_Tag].m_NextSibling; }
	int GetPrevSibling(int a_Tag) const { return m_Tags[a_Tag].m_PrevSibling; }
	int GetDataLength (int a_Tag) const { return m_Tags[a_Tag].m_DataLength; }

	const char * GetData(int a_Tag) const
	{
		ASSERT(m_Tags[a_Tag].m_Type != TAG_List);
		ASSERT(m_Tags[a_Tag].m_Type != TAG_Compound);
		return m_Data + m_Tags[a_Tag].m_DataStart;
	}
	
	int FindChildByName(int a_Tag, const AString & a_Name) const
	{
		return FindChildByName(a_Tag, a_Name.c_str(), a_Name.length());
	}
	
	int FindChildByName(int a_Tag, const char * a_Name, size_t a_NameLength = 0) const;
	int FindTagByPath  (int a_Tag, const AString & a_Path) const;
	
	eTagType GetType(int a_Tag) const { return m_Tags[a_Tag].m_Type; }
	
	/// Returns the children type for a list tag; undefined on other tags. If list empty, returns TAG_End
	eTagType GetChildrenType(int a_Tag) const
	{
		ASSERT(m_Tags[a_Tag].m_Type == TAG_List);
		return (m_Tags[a_Tag].m_FirstChild < 0) ? TAG_End : m_Tags[m_Tags[a_Tag].m_FirstChild].m_Type;
	}
	
	inline unsigned char GetByte(int a_Tag) const 
	{
		ASSERT(m_Tags[a_Tag].m_Type == TAG_Byte);
		return (unsigned char)(m_Data[m_Tags[a_Tag].m_DataStart]);
	}
	
	inline Int16 GetShort(int a_Tag) const
	{
		ASSERT(m_Tags[a_Tag].m_Type == TAG_Short);
		return ntohs(*((Int16 *)(m_Data + m_Tags[a_Tag].m_DataStart)));
	}

	inline Int32 GetInt(int a_Tag) const
	{
		ASSERT(m_Tags[a_Tag].m_Type == TAG_Int);
		return ntohl(*((Int32 *)(m_Data + m_Tags[a_Tag].m_DataStart)));
	}

	inline Int64 GetLong(int a_Tag) const
	{
		ASSERT(m_Tags[a_Tag].m_Type == TAG_Long);
		return NetworkToHostLong8(m_Data + m_Tags[a_Tag].m_DataStart);
	}

	inline float GetFloat(int a_Tag) const
	{
		ASSERT(m_Tags[a_Tag].m_Type == TAG_Float);
		Int32 tmp = ntohl(*((Int32 *)(m_Data + m_Tags[a_Tag].m_DataStart)));
		return *((float *)&tmp);
	}
	
	inline double GetDouble(int a_Tag) const
	{
		ASSERT(m_Tags[a_Tag].m_Type == TAG_Double);
		return NetworkToHostDouble8(m_Data + m_Tags[a_Tag].m_DataStart);
	}
	
protected:
	const char *             m_Data;
	int                      m_Length;
	std::vector<cFastNBTTag> m_Tags;
	bool                     m_IsValid;  // True if parsing succeeded

	// Used while parsing:
	int m_Pos;

	bool Parse(void);
	bool ReadString(int & a_StringStart, int & a_StringLen);  // Reads a simple string (2 bytes length + data), sets the string descriptors
	bool ReadCompound(void);  // Reads the latest tag as a compound
	bool ReadList(eTagType a_ChildrenType);  // Reads the latest tag as a list of items of type a_ChildrenType
	bool ReadTag(void);       // Reads the latest tag, depending on its m_Type setting
} ;





class cFastNBTWriter
{
public:
	cFastNBTWriter(void);
	
	void BeginCompound(const AString & a_Name);
	void EndCompound(void);
	
	void BeginList(const AString & a_Name, eTagType a_ChildrenType);
	void EndList(void);
	
	void AddByte     (const AString & a_Name, unsigned char a_Value);
	void AddShort    (const AString & a_Name, unsigned char a_Value);
	void AddInt      (const AString & a_Name, unsigned char a_Value);
	void AddLong     (const AString & a_Name, unsigned char a_Value);
	void AddFloat    (const AString & a_Name, unsigned char a_Value);
	void AddDouble   (const AString & a_Name, unsigned char a_Value);
	void AddString   (const AString & a_Name, unsigned char a_Value);
	void AddByteArray(const AString & a_Name, unsigned char a_Value);
	void AddIntArray (const AString & a_Name, unsigned char a_Value);

	const AString & GetResult(void) const {return m_Result; }
	
protected:
	AString m_Result;
} ;




