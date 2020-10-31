
#include "Globals.h"

#include "NBT.h"

#include <utility>
#include "FastNBT.h"

cNBT::cNBT()
{
	m_Content = new NBT::cCompound();
}





cNBT::cNBT(NBT::cNBTContent a_Content)
{
	m_Content = std::move(a_Content);
}





cNBT::~cNBT()
{
	if (static_cast<eTagType>(m_Content.index()) == TAG_Compound)
	{
		delete [] std::get<NBT::cCompound *>(m_Content);
	}
}





const NBT::cNBTContent & cNBT::Expose() const
{
	return m_Content;
}





void cNBT::Push(NBT::cNBTContent a_NewContent)
{
	assert(std::holds_alternative<NBT::cList>(m_Content));

	auto & List = std::get<NBT::cList>(m_Content);
	assert(
		List.empty() ||
		List.back().m_Content.index() == a_NewContent.index()
	);

	List.emplace_back(std::move(a_NewContent));
}





void cNBT::Serialize(const NBT::cCompound * a_Compound, cFastNBTWriter & a_Writer)
{
	a_Writer.BeginCompound("");
	for (const auto & Entry : *a_Compound)
	{
		SerializeEntry(Entry.first, Entry.second.second, Entry.second.first, a_Writer);
	}
	a_Writer.EndCompound();
}





cNBT cNBT::Deserialize(const cParsedNBT & a_NBT, int a_TagIdx)
{
	if (!a_NBT.IsValid())
	{
		return cNBT(NBT::cEmptyTag());
	}
	auto Compound = cNBT();
	int CurrentLine = a_NBT.GetFirstChild(a_TagIdx);
	while (CurrentLine > -1)
	{
		auto Type = a_NBT.GetType(CurrentLine);
		if (Type == eTagType::TAG_End)
		{
			break;
		}
		std::get<NBT::cCompound *>(Compound.Expose())->operator[](a_NBT.GetName(CurrentLine)) = {Type, DeserializeEntry(a_NBT, CurrentLine, Type)};
		CurrentLine = a_NBT.GetNextSibling(CurrentLine);
	}
	return Compound;
}





void cNBT::SerializeEntry(const AString & a_Name, const cNBT & a_Entry, eTagType a_TagType, cFastNBTWriter & a_Writer)
{
	switch (a_TagType)
	{
		case TAG_End:
		{
			break;
		}
		case TAG_Byte:
		{
			a_Writer.AddByte(a_Name, std::get<Int8>(a_Entry.Expose()));
			break;
		}
		case TAG_Short:
		{
			a_Writer.AddByte(a_Name, std::get<Int16>(a_Entry.Expose()));
			break;
		}
		case TAG_Int:
		{
			a_Writer.AddByte(a_Name, std::get<Int32>(a_Entry.Expose()));
			break;
		}
		case TAG_Long:
		{
			a_Writer.AddByte(a_Name, std::get<Int64>(a_Entry.Expose()));
			break;
		}
		case TAG_Float:
		{
			a_Writer.AddByte(a_Name, std::get<float>(a_Entry.Expose()));
			break;
		}
		case TAG_Double:
		{
			a_Writer.AddByte(a_Name, std::get<double>(a_Entry.Expose()));
			break;
		}
		case TAG_String:
		{
			a_Writer.AddString(a_Name, std::get<AString>(a_Entry.Expose()).c_str());
			break;
		}
		case TAG_List:
		{
			const auto & Array = std::get<NBT::cList>(a_Entry.Expose());
			for (const auto & ArrayEntry : Array)
			{
				auto Type = static_cast<eTagType>(ArrayEntry.Expose().index());
				a_Writer.BeginList(a_Name, Type);
				SerializeEntry("", ArrayEntry.Expose(), Type, a_Writer);
				a_Writer.EndList();
			}
			break;
		}
		case TAG_Compound:
		{
			Serialize(std::get<NBT::cCompound *>(a_Entry.Expose()), a_Writer);
			break;
		}
		case TAG_ByteArray:
		{
			const auto & Array = std::get<NBT::cByteArray>(a_Entry.Expose());
			a_Writer.AddByteArray(a_Name, Array.data(), Array.size());
			break;
		}
		case TAG_IntArray:
		{
			const auto & Array = std::get<NBT::cIntArray>(a_Entry.Expose());
			a_Writer.AddIntArray(a_Name, Array.data(), Array.size());
			break;
		}
	}
}





NBT::cNBTContent cNBT::DeserializeEntry(const cParsedNBT & a_NBT, int a_TagIdx, eTagType a_TagType)
{
	switch (a_TagType)
	{
		case TAG_Byte:   return a_NBT.GetByte(a_TagIdx);
		case TAG_Short:  return a_NBT.GetShort(a_TagIdx);
		case TAG_Int:    return a_NBT.GetInt(a_TagIdx);
		case TAG_Long:   return a_NBT.GetLong(a_TagIdx);
		case TAG_Float:  return a_NBT.GetFloat(a_TagIdx);
		case TAG_Double: return a_NBT.GetDouble(a_TagIdx);
		case TAG_ByteArray:
		{
			assert(a_NBT.GetChildrenType(a_TagIdx) == TAG_Byte);
			NBT::cByteArray ByteArray;
			int ChildLine = a_NBT.GetFirstChild(a_TagIdx);
			while (ChildLine > -1)
			{
				ByteArray.emplace_back(a_NBT.GetByte(ChildLine));
				ChildLine = a_NBT.GetNextSibling(ChildLine);
			}
			return ByteArray;
		}
		case TAG_String: return a_NBT.GetString(a_TagIdx);
		case TAG_List:
		{
			auto List = cNBT(NBT::cList());
			int ChildLine = a_NBT.GetFirstChild(a_TagIdx);
			while (ChildLine > -1)
			{
				List.Push(DeserializeEntry(a_NBT, ChildLine, a_NBT.GetType(ChildLine)));
				ChildLine = a_NBT.GetNextSibling(ChildLine);
			}
			return List.Expose();
		}
		case TAG_Compound: return Deserialize(a_NBT, a_TagIdx).Expose();
		case TAG_IntArray:
		{
			assert(a_NBT.GetChildrenType(a_TagIdx) == TAG_Int);
			NBT::cIntArray IntArray;
			int ChildLine = a_NBT.GetFirstChild(a_TagIdx);
			while (ChildLine > -1)
			{
				IntArray.emplace_back(a_NBT.GetInt(ChildLine));
				ChildLine = a_NBT.GetNextSibling(ChildLine);
			}
			return IntArray;
		}
		default: return NBT::cEmptyTag();
	}
}
