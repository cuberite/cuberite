
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
	new(&Compound) cCompound(a_Compound);
}
void cTag::uPayload::Construct(const cList & a_List)
{
	new(&List) cList(a_List);
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
	new(&Compound) cCompound(std::move(a_Compound));
}
void cTag::uPayload::Construct(cList && a_List)
{
	new(&List) cList(std::move(a_List));
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
void cTag::uPayload::Assign(cCompound && a_Compound)     { Compound = std::move(a_Compound); }
void cTag::uPayload::Assign(cList && a_List)             { List = std::move(a_List); }
void cTag::uPayload::Assign(cArray<Int8> && a_ByteArray) { ByteArray = std::move(a_ByteArray); }
void cTag::uPayload::Assign(cArray<Int32> && a_IntArray) { IntArray = std::move(a_IntArray); }

void cTag::uPayload::Assign(const AString & a_String)         { String = a_String;}
void cTag::uPayload::Assign(const cCompound & a_Compound)     { Compound = a_Compound; }
void cTag::uPayload::Assign(const cList & a_List)             { List = a_List; }
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
			// Reset to Byte to prevent double deletion
			m_TagId = TAG_Byte;
			m_Payload.Byte = 0;
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
			// Reset to Byte to prevent double deletion
			m_TagId = TAG_Byte;
			m_Payload.Byte = 0;
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
// TreeNBT::cList::Impl:

struct cList::sImpl
{
	eTagType m_TagId;
	std::vector<cTag> m_Elements;  // All elements should be of the same type as m_TagId
};





////////////////////////////////////////////////////////////////////////////////
// TreeNBT::cList:

cList::cList(eTagType a_ListType):
	m_Impl(cpp14::make_unique<sImpl>())
{
	Impl().m_TagId = a_ListType;
}





cList::cList(const cList & a_CopyFrom):
	m_Impl(cpp14::make_unique<sImpl>(a_CopyFrom.Impl()))
{
}





cList::cList(cList && a_MoveFrom):
	m_Impl(cpp14::make_unique<sImpl>(std::move(a_MoveFrom.Impl())))
{
}





cList & cList::operator = (const cList & a_CopyFrom)
{
	Impl() = a_CopyFrom.Impl();
	return *this;
}





cList & cList::operator = (cList && a_MoveFrom) NOEXCEPT
{
	swap(a_MoveFrom);
	a_MoveFrom.Impl().m_Elements.clear();
	return *this;
}





////////////////////////////////////////////////////////////////////////////////
// TreeNBT::cCompound::Impl:

struct cCompound::sImpl
{
	// Map from name->tag
	std::unordered_map<AString, cTag> m_Tags;

};





////////////////////////////////////////////////////////////////////////////////
// TreeNBT::cCompound:

cCompound::cCompound():
	m_Impl(cpp14::make_unique<sImpl>())
{
}





cCompound::cCompound(const cCompound & a_CopyFrom):
	m_Impl(cpp14::make_unique<sImpl>(a_CopyFrom.Impl()))
{
}





cCompound::cCompound(cCompound && a_MoveFrom):
	m_Impl(cpp14::make_unique<sImpl>(std::move(a_MoveFrom.Impl())))
{
}





cCompound & cCompound::operator = (const cCompound & a_CopyFrom)
{
	Impl() = a_CopyFrom.Impl();
	return *this;
}





cCompound & cCompound::operator = (cCompound && a_MoveFrom) NOEXCEPT
{
	swap(a_MoveFrom);
	a_MoveFrom.Impl().m_Tags.clear();
	return *this;
}

}  // namespace TreeNBT
