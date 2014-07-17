
// FastNBT.cpp

// Implements the fast NBT parser and writer

#include "Globals.h"
#include "FastNBT.h"





// The number of NBT tags that are reserved when an NBT parsing is started.
// You can override this by using a cmdline define
#ifndef NBT_RESERVE_SIZE
	#define NBT_RESERVE_SIZE 200
#endif  // NBT_RESERVE_SIZE

#ifdef _MSC_VER
	// Dodge a C4127 (conditional expression is constant) for this specific macro usage
	#define RETURN_FALSE_IF_FALSE(X) do { if (!X) return false; } while ((false, false))
#else
	#define RETURN_FALSE_IF_FALSE(X) do { if (!X) return false; } while (false)
#endif



////////////////////////////////////////////////////////////////////////////////
// cParsedNBT:

#define NEEDBYTES(N) \
	if (m_Length - m_Pos < (size_t)N) \
	{ \
		return false; \
	}





cParsedNBT::cParsedNBT(const char * a_Data, size_t a_Length) :
	m_Data(a_Data),
	m_Length(a_Length),
	m_Pos(0)
{
	m_IsValid = Parse();
}





bool cParsedNBT::Parse(void)
{
	if (m_Length < 3)
	{
		// Data too short
		return false;
	}
	if (m_Data[0] != TAG_Compound)
	{
		// The top-level tag must be a Compound
		return false;
	}
	
	m_Tags.reserve(NBT_RESERVE_SIZE);
	
	m_Tags.push_back(cFastNBTTag(TAG_Compound, -1));
	
	m_Pos = 1;
	
	RETURN_FALSE_IF_FALSE(ReadString(m_Tags.back().m_NameStart, m_Tags.back().m_NameLength));
	RETURN_FALSE_IF_FALSE(ReadCompound());
	
	return true;
}





bool cParsedNBT::ReadString(size_t & a_StringStart, size_t & a_StringLen)
{
	NEEDBYTES(2);
	a_StringStart = m_Pos + 2;
	a_StringLen = (size_t)GetBEShort(m_Data + m_Pos);
	if (a_StringLen > 0xffff)
	{
		// Suspicious string length
		return false;
	}
	m_Pos += 2 + a_StringLen;
	return true;
}





bool cParsedNBT::ReadCompound(void)
{
	ASSERT(m_Tags.size() > 0);

	// Reads the latest tag as a compound
	int ParentIdx = (int)m_Tags.size() - 1;
	int PrevSibling = -1;
	for (;;)
	{
		NEEDBYTES(1);
		eTagType TagType = (eTagType)(m_Data[m_Pos]);
		m_Pos++;
		if (TagType == TAG_End)
		{
			break;
		}
		m_Tags.push_back(cFastNBTTag(TagType, ParentIdx, PrevSibling));
		if (PrevSibling >= 0)
		{
			m_Tags[PrevSibling].m_NextSibling = (int)m_Tags.size() - 1;
		}
		else
		{
			m_Tags[ParentIdx].m_FirstChild = (int)m_Tags.size() - 1;
		}
		PrevSibling = (int)m_Tags.size() - 1;
		RETURN_FALSE_IF_FALSE(ReadString(m_Tags.back().m_NameStart, m_Tags.back().m_NameLength));
		RETURN_FALSE_IF_FALSE(ReadTag());
	}  // while (true)
	m_Tags[ParentIdx].m_LastChild = PrevSibling;
	return true;
}





bool cParsedNBT::ReadList(eTagType a_ChildrenType)
{
	// Reads the latest tag as a list of items of type a_ChildrenType
	
	// Read the count:
	NEEDBYTES(4);
	int Count = GetBEInt(m_Data + m_Pos);
	m_Pos += 4;
	if (Count < 0)
	{
		return false;
	}

	// Read items:
	int ParentIdx = (int)m_Tags.size() - 1;
	int PrevSibling = -1;
	for (int i = 0; i < Count; i++)
	{
		m_Tags.push_back(cFastNBTTag(a_ChildrenType, ParentIdx, PrevSibling));
		if (PrevSibling >= 0)
		{
			m_Tags[PrevSibling].m_NextSibling = (int)m_Tags.size() - 1;
		}
		else
		{
			m_Tags[ParentIdx].m_FirstChild = (int)m_Tags.size() - 1;
		}
		PrevSibling = (int)m_Tags.size() - 1;
		RETURN_FALSE_IF_FALSE(ReadTag());
	}  // for (i)
	m_Tags[ParentIdx].m_LastChild = PrevSibling;
	return true;
}





#define CASE_SIMPLE_TAG(TAGTYPE, LEN) \
	case TAG_##TAGTYPE: \
	{ \
		NEEDBYTES(LEN); \
		Tag.m_DataStart = m_Pos; \
		Tag.m_DataLength = LEN; \
		m_Pos += LEN; \
		return true; \
	}
	
bool cParsedNBT::ReadTag(void)
{
	cFastNBTTag & Tag = m_Tags.back();
	switch (Tag.m_Type)
	{
		CASE_SIMPLE_TAG(Byte,   1)
		CASE_SIMPLE_TAG(Short,  2)
		CASE_SIMPLE_TAG(Int,    4)
		CASE_SIMPLE_TAG(Long,   8)
		CASE_SIMPLE_TAG(Float,  4)
		CASE_SIMPLE_TAG(Double, 8)
		
		case TAG_String:
		{
			return ReadString(Tag.m_DataStart, Tag.m_DataLength);
		}
		
		case TAG_ByteArray:
		{
			NEEDBYTES(4);
			int len = GetBEInt(m_Data + m_Pos);
			m_Pos += 4;
			if (len < 0)
			{
				// Invalid length
				return false;
			}
			NEEDBYTES(len);
			Tag.m_DataLength = len;
			Tag.m_DataStart = m_Pos;
			m_Pos += len;
			return true;
		}
		
		case TAG_List:
		{
			NEEDBYTES(1);
			eTagType ItemType = (eTagType)m_Data[m_Pos];
			m_Pos++;
			RETURN_FALSE_IF_FALSE(ReadList(ItemType));
			return true;
		}
		
		case TAG_Compound:
		{
			RETURN_FALSE_IF_FALSE(ReadCompound());
			return true;
		}
		
		case TAG_IntArray:
		{
			NEEDBYTES(4);
			int len = GetBEInt(m_Data + m_Pos);
			m_Pos += 4;
			if (len < 0)
			{
				// Invalid length
				return false;
			}
			len *= 4;
			NEEDBYTES(len);
			Tag.m_DataLength = len;
			Tag.m_DataStart = m_Pos;
			m_Pos += len;
			return true;
		}
		
		default:
		{
			ASSERT(!"Unhandled NBT tag type");
			return false;
		}
	}  // switch (iType)
}

#undef CASE_SIMPLE_TAG





int cParsedNBT::FindChildByName(int a_Tag, const char * a_Name, size_t a_NameLength) const
{
	if (a_Tag < 0)
	{
		return -1;
	}
	if (m_Tags[a_Tag].m_Type != TAG_Compound)
	{
		return -1;
	}
	
	if (a_NameLength == 0)
	{
		a_NameLength = strlen(a_Name);
	}
	for (int Child = m_Tags[a_Tag].m_FirstChild; Child != -1; Child = m_Tags[Child].m_NextSibling)
	{
		if (
			(m_Tags[Child].m_NameLength == a_NameLength) &&
			(memcmp(m_Data + m_Tags[Child].m_NameStart, a_Name, a_NameLength) == 0)
		)
		{
			return Child;
		}
	}  // for Child - children of a_Tag
	return -1;
}





int cParsedNBT::FindTagByPath(int a_Tag, const AString & a_Path) const
{
	if (a_Tag < 0)
	{
		return -1;
	}
	size_t Begin = 0;
	size_t Length = a_Path.length();
	int Tag = a_Tag;
	for (size_t i = 0; i < Length; i++)
	{
		if (a_Path[i] != '\\')
		{
			continue;
		}
		Tag = FindChildByName(Tag, a_Path.c_str() + Begin, i - Begin - 1);
		if (Tag < 0)
		{
			return -1;
		}
		Begin = i + 1;
	}  // for i - a_Path[]
	
	if (Begin < Length)
	{
		Tag = FindChildByName(Tag, a_Path.c_str() + Begin, Length - Begin);
	}
	return Tag;
}





////////////////////////////////////////////////////////////////////////////////
// cFastNBTWriter:

cFastNBTWriter::cFastNBTWriter(const AString & a_RootTagName) :
	m_CurrentStack(0)
{
	m_Stack[0].m_Type = TAG_Compound;
	m_Result.reserve(100 * 1024);
	m_Result.push_back(TAG_Compound);
	WriteString(a_RootTagName.data(), (UInt16)a_RootTagName.size());
}





void cFastNBTWriter::BeginCompound(const AString & a_Name)
{
	if (m_CurrentStack >= MAX_STACK - 1)
	{
		ASSERT(!"Stack overflow");
		return;
	}
	
	TagCommon(a_Name, TAG_Compound);
	
	++m_CurrentStack;
	m_Stack[m_CurrentStack].m_Type = TAG_Compound;
}





void cFastNBTWriter::EndCompound(void)
{
	ASSERT(m_CurrentStack > 0);
	ASSERT(IsStackTopCompound());
	
	m_Result.push_back(TAG_End);
	--m_CurrentStack;
}





void cFastNBTWriter::BeginList(const AString & a_Name, eTagType a_ChildrenType)
{
	if (m_CurrentStack >= MAX_STACK - 1)
	{
		ASSERT(!"Stack overflow");
		return;
	}
	
	TagCommon(a_Name, TAG_List);
		
	m_Result.push_back((char)a_ChildrenType);
	m_Result.append(4, (char)0);
	
	++m_CurrentStack;
	m_Stack[m_CurrentStack].m_Type     = TAG_List;
	m_Stack[m_CurrentStack].m_Pos      = (int)m_Result.size() - 4;
	m_Stack[m_CurrentStack].m_Count    = 0;
	m_Stack[m_CurrentStack].m_ItemType = a_ChildrenType;
}





void cFastNBTWriter::EndList(void)
{
	ASSERT(m_CurrentStack > 0);
	ASSERT(m_Stack[m_CurrentStack].m_Type == TAG_List);
	
	// Update the list count:
	SetBEInt((char *)(m_Result.c_str() + m_Stack[m_CurrentStack].m_Pos), m_Stack[m_CurrentStack].m_Count);

	--m_CurrentStack;
}





void cFastNBTWriter::AddByte(const AString & a_Name, unsigned char a_Value)
{
	TagCommon(a_Name, TAG_Byte);
	m_Result.push_back(a_Value);
}





void cFastNBTWriter::AddShort(const AString & a_Name, Int16 a_Value)
{
	TagCommon(a_Name, TAG_Short);
	Int16 Value = htons(a_Value);
	m_Result.append((const char *)&Value, 2);
}





void cFastNBTWriter::AddInt(const AString & a_Name, Int32 a_Value)
{
	TagCommon(a_Name, TAG_Int);
	Int32 Value = htonl(a_Value);
	m_Result.append((const char *)&Value, 4);
}





void cFastNBTWriter::AddLong(const AString & a_Name, Int64 a_Value)
{
	TagCommon(a_Name, TAG_Long);
	Int64 Value = HostToNetwork8(&a_Value);
	m_Result.append((const char *)&Value, 8);
}





void cFastNBTWriter::AddFloat(const AString & a_Name, float a_Value)
{
	TagCommon(a_Name, TAG_Float);
	Int32 Value = HostToNetwork4(&a_Value);
	m_Result.append((const char *)&Value, 4);
}





void cFastNBTWriter::AddDouble(const AString & a_Name, double a_Value)
{
	TagCommon(a_Name, TAG_Double);
	Int64 Value = HostToNetwork8(&a_Value);
	m_Result.append((const char *)&Value, 8);
}





void cFastNBTWriter::AddString(const AString & a_Name, const AString & a_Value)
{
	TagCommon(a_Name, TAG_String);
	Int16 len = htons((short)(a_Value.size()));
	m_Result.append((const char *)&len, 2);
	m_Result.append(a_Value.c_str(), a_Value.size());
}





void cFastNBTWriter::AddByteArray(const AString & a_Name, const char * a_Value, size_t a_NumElements)
{
	TagCommon(a_Name, TAG_ByteArray);
	u_long len = htonl((u_long)a_NumElements);
	m_Result.append((const char *)&len, 4);
	m_Result.append(a_Value, a_NumElements);
}





void cFastNBTWriter::AddIntArray(const AString & a_Name, const int * a_Value, size_t a_NumElements)
{
	TagCommon(a_Name, TAG_IntArray);
	u_long len = htonl((u_long)a_NumElements);
	size_t cap = m_Result.capacity();
	size_t size = m_Result.length();
	if ((cap - size) < (4 + a_NumElements * 4))
	{
		m_Result.reserve(size + 4 + (a_NumElements * 4));
	}
	m_Result.append((const char *)&len, 4);
	for (size_t i = 0; i < a_NumElements; i++)
	{
		int Element = htonl(a_Value[i]);
		m_Result.append((const char *)&Element, 4);
	}
}





void cFastNBTWriter::Finish(void)
{
	ASSERT(m_CurrentStack == 0);
	m_Result.push_back(TAG_End);
}





void cFastNBTWriter::WriteString(const char * a_Data, UInt16 a_Length)
{
	Int16 Len = htons(a_Length);
	m_Result.append((const char *)&Len, 2);
	m_Result.append(a_Data, a_Length);
}




