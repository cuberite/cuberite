
// FastNBT.cpp

// Implements the fast NBT parser and writer

#include "Globals.h"
#include "FastNBT.h"





/** If a list being loaded has more than this number of items, it's considered corrupted. */
static const int MAX_LIST_ITEMS = 10000;





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
	if (m_Length - m_Pos < static_cast<size_t>(N)) \
	{ \
		return false; \
	}





cParsedNBT::cParsedNBT(const std::basic_string<Byte> & a_Data) :
	m_Data(a_Data.c_str()),
	m_Length(a_Data.length()),
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

	if (m_Data == nullptr)
	{
		// Invalid Data
		return false;
	}

	if (m_Data[0] != TAG_Compound)
	{
		// The top-level tag must be a Compound
		return false;
	}
	
	m_Tags.reserve(NBT_RESERVE_SIZE);
	
	m_Tags.push_back(cFastNBTTag(TAG_Compound, Option<size_t>::None()));
	
	m_Pos = 1;
	
	RETURN_FALSE_IF_FALSE(ReadString(m_Tags.back().m_NameStart, m_Tags.back().m_NameLength));
	RETURN_FALSE_IF_FALSE(ReadCompound());
	
	return true;
}





bool cParsedNBT::ReadString(size_t & a_StringStart, size_t & a_StringLen)
{
	NEEDBYTES(2);
	a_StringStart = m_Pos + 2;
	a_StringLen = static_cast<size_t>(GetBEShort(m_Data + m_Pos));
	NEEDBYTES(2 + a_StringLen);
	m_Pos += 2 + a_StringLen;
	return true;
}





bool cParsedNBT::ReadCompound(void)
{
	ASSERT(m_Tags.size() > 0);

	// Reads the latest tag as a compound
	size_t ParentIdx = m_Tags.size() - 1;
	Option<size_t> PrevSibling = Option<size_t>::None();
	for (;;)
	{
		NEEDBYTES(1);
		const auto TagTypeNum = m_Data[m_Pos];
		if (TagTypeNum > TAG_Max)
		{
			return false;
		}
		eTagType TagType = static_cast<eTagType>(TagTypeNum);
		m_Pos++;
		if (TagType == TAG_End)
		{
			break;
		}
		m_Tags.push_back(cFastNBTTag(TagType, ParentIdx, PrevSibling));
		if (PrevSibling.HasValue())
		{
			m_Tags[PrevSibling.GetValue()].m_NextSibling = m_Tags.size() - 1;
		}
		else
		{
			m_Tags[ParentIdx].m_FirstChild = m_Tags.size() - 1;
		}
		PrevSibling = m_Tags.size() - 1;
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
	if ((Count < 0) || (Count > MAX_LIST_ITEMS))
	{
		return false;
	}

	// Read items:
	ASSERT(m_Tags.size() > 0);
	size_t ParentIdx = m_Tags.size() - 1;
	Option<size_t> PrevSibling = Option<size_t>::None();
	for (int i = 0; i < Count; i++)
	{
		m_Tags.push_back(cFastNBTTag(a_ChildrenType, ParentIdx, PrevSibling));
		if (PrevSibling.HasValue())
		{
			m_Tags[PrevSibling.GetValue()].m_NextSibling = m_Tags.size() - 1;
		}
		else
		{
			m_Tags[ParentIdx].m_FirstChild = m_Tags.size() - 1;
		}
		PrevSibling = m_Tags.size() - 1;
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
			Tag.m_DataLength = static_cast<size_t>(len);
			Tag.m_DataStart = m_Pos;
			m_Pos += static_cast<size_t>(len);
			return true;
		}
		
		case TAG_List:
		{
			NEEDBYTES(1);
			eTagType ItemType = static_cast<eTagType>(m_Data[m_Pos]);
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
			Tag.m_DataLength = static_cast<size_t>(len);
			Tag.m_DataStart = m_Pos;
			m_Pos += static_cast<size_t>(len);
			return true;
		}
		
		#if !defined(__clang__)
		default:
		#endif
		case TAG_Min:
		{
			return false;
		}
	}  // switch (iType)
}

#undef CASE_SIMPLE_TAG





Option<size_t> cParsedNBT::FindChildByName(size_t a_Tag, const char * a_Name, size_t a_NameLength) const
{
	if (m_Tags[a_Tag].m_Type != TAG_Compound)
	{
		return Option<size_t>::None();
	}
	
	if (a_NameLength == 0)
	{
		a_NameLength = strlen(a_Name);
	}
	for (auto Child = m_Tags[a_Tag].m_FirstChild; Child.HasValue(); Child = m_Tags[Child.GetValue()].m_NextSibling)
	{
		if (
			(m_Tags[Child.GetValue()].m_NameLength == a_NameLength) &&
			(memcmp(m_Data + m_Tags[Child.GetValue()].m_NameStart, a_Name, a_NameLength) == 0)
		)
		{
			return Child.GetValue();
		}
	}  // for Child - children of a_Tag
	return Option<size_t>::None();
}





Option<size_t> cParsedNBT::FindTagByPath(size_t a_Tag, const AString & a_Path) const
{
	size_t Begin = 0;
	size_t Length = a_Path.length();
	size_t Tag = a_Tag;
	for (size_t i = 0; i < Length; i++)
	{
		if (a_Path[i] != '\\')
		{
			continue;
		}
		auto MaybeTag = FindChildByName(Tag, a_Path.c_str() + Begin, i - Begin);
		if (!MaybeTag.HasValue())
		{
			return Tag;
		}
		Tag = MaybeTag.GetValue();
		Begin = i + 1;
	}  // for i - a_Path[]
	
	if (Begin < Length)
	{
		return FindChildByName(Tag, a_Path.c_str() + Begin, Length - Begin);
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
	WriteString(a_RootTagName.data(), static_cast<UInt16>(a_RootTagName.size()));
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
		
	m_Result.push_back(static_cast<Byte>(a_ChildrenType));
	m_Result.append(4, static_cast<char>(0));
	
	++m_CurrentStack;
	m_Stack[m_CurrentStack].m_Type     = TAG_List;
	m_Stack[m_CurrentStack].m_Pos      = m_Result.size() - 4;
	m_Stack[m_CurrentStack].m_Count    = 0;
	m_Stack[m_CurrentStack].m_ItemType = a_ChildrenType;
}





void cFastNBTWriter::EndList(void)
{
	ASSERT(m_CurrentStack > 0);
	ASSERT(m_Stack[m_CurrentStack].m_Type == TAG_List);
	
	// Update the list count:
	SetBEInt(m_Result, m_Stack[m_CurrentStack].m_Pos, m_Stack[m_CurrentStack].m_Count);

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
	UInt16 Value = htons(static_cast<UInt16>(a_Value));
	m_Result.append(reinterpret_cast<const Byte *>(&Value), 2);
}





void cFastNBTWriter::AddInt(const AString & a_Name, Int32 a_Value)
{
	TagCommon(a_Name, TAG_Int);
	UInt32 Value = htonl(static_cast<UInt32>(a_Value));
	m_Result.append(reinterpret_cast<const Byte *>(&Value), 4);
}





void cFastNBTWriter::AddLong(const AString & a_Name, Int64 a_Value)
{
	TagCommon(a_Name, TAG_Long);
	UInt64 Value = HostToNetwork8(&a_Value);
	m_Result.append(reinterpret_cast<const Byte *>(&Value), 8);
}





void cFastNBTWriter::AddFloat(const AString & a_Name, float a_Value)
{
	TagCommon(a_Name, TAG_Float);
	UInt32 Value = HostToNetwork4(&a_Value);
	m_Result.append(reinterpret_cast<const Byte *>(&Value), 4);
}





void cFastNBTWriter::AddDouble(const AString & a_Name, double a_Value)
{
	TagCommon(a_Name, TAG_Double);
	UInt64 Value = HostToNetwork8(&a_Value);
	m_Result.append(reinterpret_cast<const Byte *>(&Value), 8);
}





void cFastNBTWriter::AddString(const AString & a_Name, const AString & a_Value)
{
	TagCommon(a_Name, TAG_String);
	UInt16 len = htons(static_cast<UInt16>(a_Value.size()));
	m_Result.append(reinterpret_cast<const Byte *>(&len), 2);
	m_Result.append(reinterpret_cast<const Byte *>(a_Value.c_str()), a_Value.size());
}





void cFastNBTWriter::AddByteArray(const AString & a_Name, const Byte * a_Value, size_t a_NumElements)
{
	TagCommon(a_Name, TAG_ByteArray);
	u_int len = htonl(static_cast<u_int>(a_NumElements));
	m_Result.append(reinterpret_cast<const Byte *>(&len), 4);
	m_Result.append(a_Value, a_NumElements);
}





void cFastNBTWriter::AddIntArray(const AString & a_Name, const int * a_Value, size_t a_NumElements)
{
	TagCommon(a_Name, TAG_IntArray);
	u_int len = htonl(static_cast<u_int>(a_NumElements));
	size_t cap = m_Result.capacity();
	size_t size = m_Result.length();
	if ((cap - size) < (4 + a_NumElements * 4))
	{
		m_Result.reserve(size + 4 + (a_NumElements * 4));
	}
	m_Result.append(reinterpret_cast<const Byte *>(&len), 4);
	for (size_t i = 0; i < a_NumElements; i++)
	{
		UInt32 Element = htonl(static_cast<UInt32>(a_Value[i]));
		m_Result.append(reinterpret_cast<const Byte *>(&Element), 4);
	}
}





void cFastNBTWriter::Finish(void)
{
	ASSERT(m_CurrentStack == 0);
	m_Result.push_back(TAG_End);
}





void cFastNBTWriter::WriteString(const char * a_Data, UInt16 a_Length)
{
	UInt16 Len = htons(a_Length);
	m_Result.append(reinterpret_cast<const Byte *>(&Len), 2);
	m_Result.append(reinterpret_cast<const Byte *>(a_Data), a_Length);
}




