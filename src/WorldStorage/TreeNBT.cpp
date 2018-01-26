
#include "Globals.h"

#include "WorldStorage/TreeNBT.h"

#include <unordered_map>


namespace
{

struct sDestroyer
{
	template <typename T>
	void operator () (T & a_Value)
	{
		a_Value.~T();
	}

	void operator () (TreeNBT::cCompound & a_Compound)
	{
		delete &a_Compound;
	}

	void operator () (TreeNBT::cList & a_Compound)
	{
		delete &a_Compound;
	}

	void operator () (TreeNBT::cEmptyTag &) {}
};

}  // namespace (anonymous)



namespace TreeNBT
{

#pragma push_macro("new")
#undef new

////////////////////////////////////////////////////////////////////////////////
// TreeNBT::cTag::uPayload:

void cTag::uPayload::Construct(const AString & a_String)
{
	new(&String) AString(a_String);
}
void cTag::uPayload::Construct(const cCompound & a_Compound)
{
	Compound = new cCompound(a_Compound);
}
void cTag::uPayload::Construct(const cList & a_List)
{
	List = new cList(a_List);
}
void cTag::uPayload::Construct(const cArray<Int8> & a_ByteArray)
{
	new(&ByteArray) cArray<Int8>(a_ByteArray);
}
void cTag::uPayload::Construct(const cArray<Int32> & a_IntArray)
{
	new(&IntArray) cArray<Int32>(a_IntArray);
}

void cTag::uPayload::Construct(AString && a_String)
{
	new(&String) AString(std::move(a_String));
}
void cTag::uPayload::Construct(cCompound && a_Compound)
{
	Compound = new cCompound(std::move(a_Compound));
}
void cTag::uPayload::Construct(cList && a_List)
{
	List = new cList(std::move(a_List));
}
void cTag::uPayload::Construct(cArray<Int8> && a_ByteArray)
{
	new(&ByteArray) cArray<Int8>(std::move(a_ByteArray));
}
void cTag::uPayload::Construct(cArray<Int32> && a_IntArray)
{
	new(&IntArray) cArray<Int32>(std::move(a_IntArray));
}

void cTag::uPayload::Assign(AString && a_String)         { String = std::move(a_String); }
void cTag::uPayload::Assign(cCompound && a_Compound)     { *Compound = std::move(a_Compound); }
void cTag::uPayload::Assign(cList && a_List)             { *List = std::move(a_List); }
void cTag::uPayload::Assign(cArray<Int8> && a_ByteArray) { ByteArray = std::move(a_ByteArray); }
void cTag::uPayload::Assign(cArray<Int32> && a_IntArray) { IntArray = std::move(a_IntArray); }

void cTag::uPayload::Assign(const AString & a_String)         { String = a_String;}
void cTag::uPayload::Assign(const cCompound & a_Compound)     { *Compound = a_Compound; }
void cTag::uPayload::Assign(const cList & a_List)             { *List = a_List; }
void cTag::uPayload::Assign(const cArray<Int8> & a_ByteArray) { ByteArray = a_ByteArray; }
void cTag::uPayload::Assign(const cArray<Int32> & a_IntArray) { IntArray = a_IntArray; }

#pragma pop_macro("new")





////////////////////////////////////////////////////////////////////////////////
// TreeNBT::cTag:

void cTag::Destroy()
{
	Visit(sDestroyer{});
}





cTag::cTag(const cTag & a_CopyFrom):
	m_TagId{a_CopyFrom.m_TagId},
	m_Payload{Int8{0}}
{
	a_CopyFrom.Visit(sConstructor{m_Payload});
}





cTag::cTag(cTag && a_MoveFrom):
	m_TagId{a_MoveFrom.m_TagId},
	m_Payload{Int8{0}}
{
	std::move(a_MoveFrom).Visit(sConstructor{m_Payload});
}





cTag & cTag::operator = (const cTag & a_CopyFrom)
{
	if (m_TagId != a_CopyFrom.m_TagId)
	{
		// Copy assign different type
		Destroy();
		m_TagId = a_CopyFrom.m_TagId;

		try
		{
			a_CopyFrom.Visit(sConstructor{m_Payload});
		}
		catch (...)
		{
			// We just deleted the old value but the new value failed to construct
			// Tag as empty to avoid double deletion
			m_TagId = TAG_End;
			throw;
		}
	}
	else
	{
		// Copy assign same type
		a_CopyFrom.Visit(sAssigner{m_Payload});
	}
	return *this;
}






cTag & cTag::operator = (cTag && a_MoveFrom)
{
	if (m_TagId != a_MoveFrom.m_TagId)
	{
		// Move assign different type
		Destroy();
		m_TagId = a_MoveFrom.m_TagId;

		try
		{
			std::move(a_MoveFrom).Visit(sConstructor{m_Payload});
		}
		catch (...)
		{
			// We just deleted the old value but the new value failed to construct
			// Tag as empty to avoid double deletion
			m_TagId = TAG_End;
			throw;
		}
	}
	else
	{
		// Move assign same type
		std::move(a_MoveFrom).Visit(sAssigner{m_Payload});
	}
	return *this;
}





////////////////////////////////////////////////////////////////////////////////
// TreeNBT::cTag:

void cList::resize(size_type a_NewSize)
{
	auto OldSize = size();

	if (OldSize >= a_NewSize)
	{
		// Resize without any new elements
		m_Tags.resize(a_NewSize);
	}
	else
	{
		// Resize with new elements
		auto DefaultValue = [&]
			{
				switch (m_TypeId)
				{
					case TAG_Byte:      return cTag(Int8{});
					case TAG_Short:     return cTag(Int16{});
					case TAG_Int:       return cTag(Int32{});
					case TAG_Long:      return cTag(Int64{});
					case TAG_Float:     return cTag(float{});
					case TAG_Double:    return cTag(double{});
					case TAG_ByteArray: return cTag(cArray<Int8>{});
					case TAG_String:    return cTag(AString{});
					case TAG_List:      return cTag(cList{TAG_Byte});
					case TAG_Compound:  return cTag(cCompound{});
					case TAG_IntArray:  return cTag(cArray<Int32>{});
					case TAG_End:
					{
						ASSERT(!"Bad array type id");
						return cTag();
					}
				}
			}();
		m_Tags.resize(a_NewSize, DefaultValue);
	}
}

}  // namespace TreeNBT
