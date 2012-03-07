
// NBT.cpp

// Implements the classes used for NBT representation, parsing and serializing

/*
This file has been retrofitted from a different project of mine (_Xoft(o)) and melded here, so it is not strictyl MCS-styled
Also the project used error codes, which MCS doesn't need, so they were declared locally only.
The code is not strictly safe, it could leak pointers when exceptions are thrown. It could use a RAII redesign.
*/

#include "Globals.h"
#include "NBT.h"
#include "Endianness.h"





// Error codes
// Unused by MCS
enum
{
	ERROR_PRIVATE_BASE = 0x00040000,
	
	ERROR_PRIVATE_NBTPARSER_BADTYPE,         // The parsed type is not recognized
	ERROR_PRIVATE_NBTPARSER_INVALIDLENGTH,   // The parsed name has an invalid length (negative-length string etc.)
	ERROR_PRIVATE_NBTPARSER_UNEXPECTEDTAG,   // The parser has encountered a tag that should not be at such a point
	ERROR_PRIVATE_NBTPARSER_TOOSHORT,        // The parser was invoked on data that is too short
	
	ERROR_PRIVATE_NBT_UNINITIALIZEDLIST,     // NBTList needs its ChildType set before adding items to it
	ERROR_PRIVATE_NBT_TYPEMISMATCH,          // NBTList must have all of its children of the same type
	ERROR_PRIVATE_NBT_UNEXPECTEDTAG,         // NBTList and NBTCompound cannot contain a TAG_End
	ERROR_PRIVATE_NBT_BADTYPE,               // NBTList's children type cannot be set to such a value
	ERROR_PRIVATE_NBT_LISTNOTEMPTY,          // NBTList must be empty to allow setting children type
	ERROR_PRIVATE_NBT_INDEXOUTOFRANGE,       // Requesting an item with invalid index from a list or a compound

	ERROR_PRIVATE_UNKNOWN,                   // Unknown error
} ;

#ifndef ERROR_SUCCESS
	// This constant is actually #defined in the WinAPI; it cannot be put into the above enum, but needs to be #defined (for *nix)
	#define ERROR_SUCCESS 0
#endif  // ERROR_SUCCCESS

#ifndef ERROR_NOT_ENOUGH_MEMORY
	// This constant is actually #defined in the WinAPI; it cannot be put into the above enum, but needs to be #defined (for *nix)
	#define ERROR_NOT_ENOUGH_MEMORY 8
#endif  // ERROR_NOT_ENOUGH_MEMORY





#define RETURN_INT_IF_FAILED(X) {int r = X; if (r != ERROR_SUCCESS) return r; }





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cNBTTag:

cNBTTag * cNBTTag::CreateTag(cNBTTag * a_Parent, eTagType a_Type, const AString & a_Name)
{
	// Creates a new instance of a tag specified by a_Type, uses the correct class
	switch (a_Type)
	{
		case TAG_Byte:      return new cNBTByte     (a_Parent, a_Name);
		case TAG_Short:     return new cNBTShort    (a_Parent, a_Name);
		case TAG_Int:       return new cNBTInt      (a_Parent, a_Name);
		case TAG_Long:      return new cNBTLong     (a_Parent, a_Name);
		case TAG_Float:     return new cNBTFloat    (a_Parent, a_Name);
		case TAG_Double:    return new cNBTDouble   (a_Parent, a_Name);
		case TAG_ByteArray: return new cNBTByteArray(a_Parent, a_Name);
		case TAG_String:    return new cNBTString   (a_Parent, a_Name);
		case TAG_List:      return new cNBTList     (a_Parent, a_Name);
		case TAG_Compound:  return new cNBTCompound (a_Parent, a_Name);
		default:
		{
			ASSERT("Unknown TAG type requested" == NULL);
			return NULL;
		}
	}
}





const cNBTTag * cNBTTag::FindChildByPath(const AString & iPath) const
{
	size_t PrevIdx = 0;
	size_t idx = iPath.find('\\');
	const cNBTTag * res = this;
	while ((res != NULL) && (idx != AString::npos))
	{
		res = res->FindChildByName(AString(iPath, PrevIdx, idx - PrevIdx));
		PrevIdx = idx + 1;
		idx = iPath.find('\\', PrevIdx);
	}
	if (res != NULL)
	{
		res = res->FindChildByName(AString(iPath, PrevIdx));
	}
	return res;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cNBTList:

void cNBTList::Clear(void)
{
	for (cNBTTags::iterator itr = m_Children.begin(); itr != m_Children.end(); ++itr)
	{
		delete *itr;
	}
	m_Children.clear();
}




int cNBTList::Add(cNBTTag * iTag)
{
	// Catch usage errors while debugging:
	ASSERT(m_ChildrenType != TAG_End);
	ASSERT(iTag->GetType() == m_ChildrenType);
	
	// Catch errors while running:
	if (m_ChildrenType == TAG_End)
	{
		return ERROR_PRIVATE_NBT_UNINITIALIZEDLIST;
	}
	if (iTag->GetType() != m_ChildrenType)
	{
		return ERROR_PRIVATE_NBT_TYPEMISMATCH;
	}
	
	m_Children.push_back(iTag);
	return ERROR_SUCCESS;
}





int cNBTList::SetChildrenType(cNBTTag::eTagType a_Type)
{
	// Catch usage errors while debugging:
	ASSERT(a_Type != TAG_End);  // Invalid, though not specifically in the NBT spec
	ASSERT(m_Children.size() == 0);  // Can change only when empty
	
	// Catch runtime errors:
	if (a_Type == TAG_End)
	{
		return ERROR_PRIVATE_NBT_BADTYPE;
	}
	if (m_Children.size() != 0)
	{
		return ERROR_PRIVATE_NBT_LISTNOTEMPTY;
	}
	
	m_ChildrenType = a_Type;
	return ERROR_SUCCESS;
}





cNBTTag * cNBTList::GetChildByIdx(size_t iIndex)
{
	// Catch usage errors while debugging:
	ASSERT((iIndex >= 0) && (iIndex < m_Children.size()));
	
	// Catch runtime errors:
	if ((iIndex < 0) || (iIndex >= m_Children.size()))
	{
		return NULL;
	}
	
	return m_Children[iIndex];
}





cNBTTag * cNBTList::FindChildByName(const AString & a_Name) const
{
	for (cNBTTags::const_iterator itr = m_Children.begin(); itr != m_Children.end(); ++itr)
	{
		if ((*itr)->GetName() == a_Name)
		{
			return *itr;
		}
	}
	return NULL;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cNBTCompound:

void cNBTCompound::Clear(void)
{
	for (cNBTTags::iterator itr = m_Children.begin(); itr != m_Children.end(); ++itr)
	{
		delete *itr;
	}
	m_Children.clear();
}





int cNBTCompound::Add(cNBTTag * iTag)
{
	// Catch usage errors while debugging:
	ASSERT(iTag->GetType() != TAG_End);
	
	// Catch runtime errors:
	if (iTag->GetType() == TAG_End)
	{
		return ERROR_PRIVATE_NBT_UNEXPECTEDTAG;
	}
	
	m_Children.push_back(iTag);
	return ERROR_SUCCESS;
}





cNBTTag * cNBTCompound::GetChildByIdx(size_t iIndex)
{
	// Catch usage errors while debugging:
	ASSERT((iIndex >= 0) && (iIndex < m_Children.size()));
	
	// Catch runtime errors:
	if ((iIndex < 0) || (iIndex >= m_Children.size()))
	{
		return NULL;
	}
	
	return m_Children[iIndex];
}





cNBTTag * cNBTCompound::FindChildByName(const AString & a_Name) const
{
	for (cNBTTags::const_iterator itr = m_Children.begin(); itr != m_Children.end(); ++itr)
	{
		if ((*itr)->GetName() == a_Name)
		{
			return *itr;
		}
	}
	return NULL;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cNBTParser:

int cNBTParser::ReadByte(const char ** a_Data, int * a_Length, char & a_Value)
{
	if (*a_Length < 1)
	{
		return ERROR_PRIVATE_NBTPARSER_TOOSHORT;
	}
	a_Value = **a_Data;
	*a_Data += 1;
	*a_Length -= 1;
	return ERROR_SUCCESS;
}





int cNBTParser::ReadInt16(const char ** a_Data, int * a_Length, Int16 & a_Value)
{
	if (*a_Length < 2)
	{
		return ERROR_PRIVATE_NBTPARSER_TOOSHORT;
	}
	a_Value = ntohs(*((Int16 *)*a_Data));
	*a_Data += 2;
	*a_Length -= 2;
	return ERROR_SUCCESS;
}





int cNBTParser::ReadInt32(const char ** a_Data, int * a_Length, Int32 & a_Value)
{
	if (*a_Length < 4)
	{
		return ERROR_PRIVATE_NBTPARSER_TOOSHORT;
	}
	a_Value = ntohl(*((Int32 *)*a_Data));
	*a_Data += 4;
	*a_Length -= 4;
	return ERROR_SUCCESS;
}





int cNBTParser::ReadInt64(const char ** a_Data, int * a_Length, Int64 & a_Value)
{
	if (*a_Length < 8)
	{
		return ERROR_PRIVATE_NBTPARSER_TOOSHORT;
	}
	a_Value = NetworkToHostLong8(*a_Data);
	*a_Data += 8;
	*a_Length -= 8;
	return ERROR_SUCCESS;
}





int cNBTParser::ReadFloat(const char ** a_Data, int * a_Length, float & a_Value)
{
	if (*a_Length < 4)
	{
		return ERROR_PRIVATE_NBTPARSER_TOOSHORT;
	}
	// Read as a 32-bit integer, converting endianness, then reinterpret as float:
	Int32 tmp = ntohl(*((Int32 *)*a_Data));
	a_Value = *((float *)&tmp);
	*a_Data += 4;
	*a_Length -= 4;
	return ERROR_SUCCESS;
}





int cNBTParser::ReadDouble(const char ** a_Data, int * a_Length, double & a_Value)
{
	if (*a_Length < 8)
	{
		return ERROR_PRIVATE_NBTPARSER_TOOSHORT;
	}
	a_Value = NetworkToHostDouble8(a_Data);
	*a_Data += 8;
	*a_Length -= 8;
	return ERROR_SUCCESS;
}





int cNBTParser::ReadByteArray(const char ** a_Data, int * a_Length, AString & a_String)
{
	// Reads the short-counted string, adjusts a_Data and a_Length accordingly
	Int32 Len;
	RETURN_INT_IF_FAILED(ReadInt32(a_Data, a_Length, Len));
	if (Len < 0)
	{
		return ERROR_PRIVATE_NBTPARSER_INVALIDLENGTH;
	}
	if (*a_Length < Len)
	{
		return ERROR_PRIVATE_NBTPARSER_TOOSHORT;
	}
	a_String.assign(*a_Data, Len);
	*a_Data += Len;
	*a_Length -= Len;
	return ERROR_SUCCESS;
}





int cNBTParser::ReadString(const char ** a_Data, int * a_Length, AString & a_String)
{
	// Reads the short-counted string, adjusts a_Data and a_Length accordingly
	if (*a_Length < 2)
	{
		return ERROR_PRIVATE_NBTPARSER_TOOSHORT;
	}
	Int16 val = *((Int16 *)*a_Data);
	Int16 Len = ntohs(val);
	if (Len < 0)
	{
		return ERROR_PRIVATE_NBTPARSER_INVALIDLENGTH;
	}
	*a_Data += 2;
	*a_Length -= 2;
	if (*a_Length < Len)
	{
		return ERROR_PRIVATE_NBTPARSER_TOOSHORT;
	}
	a_String.assign(*a_Data, Len);
	*a_Data += Len;
	*a_Length -= Len;
	return ERROR_SUCCESS;
}





int cNBTParser::ReadList(const char ** a_Data, int * a_Length, cNBTList * a_List)
{
	// Reads a_List's     contents from a_Data; up to a_Length bytes may be used; adjusts a_Data and a_Length for after the list
	Int32 ItemCount;
	RETURN_INT_IF_FAILED(ReadInt32(a_Data, a_Length, ItemCount));
	for (Int32 i = 0; i < ItemCount; i++)
	{
		cNBTTag * child = NULL;
		RETURN_INT_IF_FAILED(ReadTag(a_Data, a_Length, a_List->GetChildrenType(), "", a_List, &child));
		if (child == NULL)
		{
			return ERROR_PRIVATE_UNKNOWN;
		}
		RETURN_INT_IF_FAILED(a_List->Add(child));
	}  // for i - Items[]
	return ERROR_SUCCESS;
}





int cNBTParser::ReadCompound(const char ** a_Data, int * a_Length, cNBTCompound * a_Compound)
{
	// Reads a_Compound's contents from a_Data; up to a_Length bytes may be used; adjusts a_Data and a_Length for after the compound
	while (true)
	{
		char TagType = **a_Data;
		*a_Data += 1;
		*a_Length -= 1;
		if (TagType == cNBTTag::TAG_End)
		{
			return ERROR_SUCCESS;
		}
		AString Name;
		RETURN_INT_IF_FAILED(ReadString(a_Data, a_Length, Name));
		cNBTTag * child = NULL;
		RETURN_INT_IF_FAILED(ReadTag(a_Data, a_Length, (cNBTTag::eTagType)TagType, Name, a_Compound, &child));
		if (child == NULL)
		{
			return ERROR_PRIVATE_UNKNOWN;
		}
		RETURN_INT_IF_FAILED(a_Compound->Add(child));
	}  // while (true)
}




int cNBTParser::ReadIntArray(const char ** a_Data, int * a_Length, cNBTIntArray * a_Array)
{
	if (*a_Length < 4)
	{
		return ERROR_PRIVATE_NBTPARSER_TOOSHORT;
	}
	int Count = ntohl(*((int *)*a_Data));
	*a_Data += 4;
	*a_Length -= 4;
	if (*a_Length < 4 * Count)
	{
		return ERROR_PRIVATE_NBTPARSER_TOOSHORT;
	}
	for (int i = 0; i < Count; i++)
	{
		int Value = ntohl(*((int *)*a_Data));
		a_Array->Add(Value);
		*a_Data += 4;
	}
	*a_Length -= 4 * Count;
	return ERROR_SUCCESS;
}





#define CASE_SIMPLE_TAG(TAGTYPE,CTYPE,FUNC) \
	case cNBTTag::TAG_##TAGTYPE: \
	{ \
		CTYPE val; \
		RETURN_INT_IF_FAILED(Read##FUNC(a_Data, a_Length, val)); \
		*a_Tag = new cNBT##TAGTYPE(a_Parent, a_Name, val); \
		if (*a_Tag == NULL) \
		{ \
			return ERROR_NOT_ENOUGH_MEMORY; \
		} \
		return ERROR_SUCCESS;\
	}
	
int cNBTParser::ReadTag(const char ** a_Data, int * a_Length, cNBTTag::eTagType a_Type, const AString & a_Name, cNBTTag * a_Parent, cNBTTag ** a_Tag)
{
	switch (a_Type)
	{
		CASE_SIMPLE_TAG(Byte,      char,    Byte)
		CASE_SIMPLE_TAG(Short,     Int16,   Int16)
		CASE_SIMPLE_TAG(Int,       Int32,   Int32)
		CASE_SIMPLE_TAG(Long,      Int64,   Int64)
		CASE_SIMPLE_TAG(Float,     float,   Float)
		CASE_SIMPLE_TAG(Double,    double,  Double)
		CASE_SIMPLE_TAG(ByteArray, AString, ByteArray)
		CASE_SIMPLE_TAG(String,    AString, String)
		
		case cNBTTag::TAG_List:
		{
			char ItemType;
			RETURN_INT_IF_FAILED(ReadByte (a_Data, a_Length, ItemType));
			cNBTList * List = new cNBTList(a_Parent, a_Name);
			if (List == NULL)
			{
				return ERROR_NOT_ENOUGH_MEMORY;
			}
			RETURN_INT_IF_FAILED(List->SetChildrenType((cNBTTag::eTagType)ItemType));
			RETURN_INT_IF_FAILED(ReadList(a_Data, a_Length, List));
			*a_Tag = List;
			return ERROR_SUCCESS;
		}
		
		case cNBTTag::TAG_Compound:
		{
			cNBTCompound * Compound = new cNBTCompound(a_Parent, a_Name);
			if (Compound == NULL)
			{
				return ERROR_NOT_ENOUGH_MEMORY;
			}
			RETURN_INT_IF_FAILED(ReadCompound(a_Data, a_Length, Compound));
			*a_Tag = Compound;
			return ERROR_SUCCESS;
		}
		
		case cNBTTag::TAG_IntArray:
		{
			cNBTIntArray * Array = new cNBTIntArray(a_Parent, a_Name);
			if (Array == NULL)
			{
				return ERROR_NOT_ENOUGH_MEMORY;
			}
			RETURN_INT_IF_FAILED(ReadIntArray(a_Data, a_Length, Array));
			*a_Tag = Array;
			return ERROR_SUCCESS;
		}
		
		default:
		{
			ASSERT(!"Unhandled NBT tag type");
			break;
		}
	}  // switch (iType)

	return ERROR_PRIVATE_NBTPARSER_BADTYPE;
}

#undef CASE_SIMPLE_TAG




cNBTTree * cNBTParser::Parse(const char * a_Data, int a_Length)
{
	// Creates a NBT from a_Data
	if (a_Length < 3)
	{
		return NULL;
	}
	if (a_Data[0] != cNBTTag::TAG_Compound)
	{
		return NULL;
	}
	a_Data++;
	a_Length--;
	AString Name;
	if (ReadString(&a_Data, &a_Length, Name) != 0)
	{
		return NULL;
	}
	std::auto_ptr<cNBTCompound> Root(new cNBTCompound(NULL, Name));
	if (Root.get() == NULL)
	{
		return NULL;
	}
	if (ReadCompound(&a_Data, &a_Length, Root.get()) == 0)
	{
		return Root.release();
	}
	return NULL;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dumping the NBT tree (debug-only)

#ifdef _DEBUG

#define CASE_SIMPLE_TAG(TYPE,FMT) \
	case cNBTTag::TAG_##TYPE: \
	{ \
		AString out; \
		Printf(out, "%sTAG_" TEXT(#TYPE) TEXT("(\"%hs\"): %") TEXT(FMT) TEXT("\n"), Indent.c_str(), a_Tree->GetName().c_str(), ((cNBT##TYPE *)a_Tree)->m_Value); \
		OutputDebugString(out.c_str()); \
		break; \
	}
	
void DumpTree(const cNBTTree * a_Tree, int a_Level)
{
	AString Indent(a_Level, TEXT(' '));
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (a_Tree->GetType())
	{
		CASE_SIMPLE_TAG(Byte,  "d")
		CASE_SIMPLE_TAG(Short, "d")
		CASE_SIMPLE_TAG(Int,   "d")
		CASE_SIMPLE_TAG(Long,   "I64d")
		CASE_SIMPLE_TAG(Float,  "f")
		CASE_SIMPLE_TAG(Double, "f")
		
		case cNBTTag::TAG_ByteArray:
		{
			AString out;
			Printf(out, "%sTAG_ByteArray(\"%hs\"): %d bytes\n", Indent.c_str(), a_Tree->GetName().c_str(), ((cNBTByteArray *)a_Tree)->m_Value.size());
			OutputDebugString(out.c_str());
			break;
		}
		
		case cNBTTag::TAG_String:
		{
			AString out;
			Printf(out, "%sTAG_String(\"%hs\"): %d bytes: \"%hs\"\n", Indent.c_str(), a_Tree->GetName().c_str(), ((cNBTString *)a_Tree)->m_Value.size(), ((cNBTString *)a_Tree)->m_Value.c_str());
			OutputDebugString(out.c_str());
			break;
		}
		
		case cNBTTag::TAG_List:
		{
			const cNBTTags & Children = ((cNBTList *)a_Tree)->GetChildren();
			AString out;
			Printf(out, "%sTAG_List(\"%hs\"): %d items of type %d\n%s{\n", Indent.c_str(), a_Tree->GetName().c_str(), Children.size(), ((cNBTList *)a_Tree)->GetChildrenType(), Indent.c_str());
			OutputDebugString(out.c_str());
			for (cNBTTags::const_iterator itr = Children.begin(); itr != Children.end(); ++itr)
			{
				DumpTree(*itr, a_Level + 1);
			}  // for itr - Children[]
			Printf(out, "%s}\n", Indent.c_str());
			OutputDebugString(out.c_str());
			break;
		}

		case cNBTTag::TAG_Compound:
		{
			const cNBTTags & Children = ((cNBTCompound *)a_Tree)->GetChildren();
			AString out;
			Printf(out, "%sTAG_Compound(\"%hs\"): %d items\n%s{\n", Indent.c_str(), a_Tree->GetName().c_str(), Children.size(), Indent.c_str());
			OutputDebugString(out.c_str());
			for (cNBTTags::const_iterator itr = Children.begin(); itr != Children.end(); ++itr)
			{
				DumpTree(*itr, a_Level + 1);
			}  // for itr - Children[]
			Printf(out, "%s}\n", Indent.c_str());
			OutputDebugString(out.c_str());
			break;
		}
	}
}

#undef CASE_SIMPLE_TAG

#endif  // _DEBUG




