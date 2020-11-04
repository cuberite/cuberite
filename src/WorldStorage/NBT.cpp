
#include "Globals.h"

#include "NBT.h"
#include "FastNBT.h"

cNBT::cNBT()
{
	m_Content = new NBT::cCompound();
}





cNBT::cNBT(NBT::cNBTContent a_Content)
{
	if (std::holds_alternative<NBT::cList>(a_Content))
	{
		VERIFY(std::get<NBT::cList>(a_Content).size() < 2);
	}
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





NBT::cCompound & cNBT::ExposeCompound() const
{
	VERIFY(std::holds_alternative<NBT::cCompound *>(m_Content));
	return *std::get<NBT::cCompound *>(m_Content);
}





void cNBT::Push(NBT::cNBTContent a_NewContent)
{
	VERIFY(std::holds_alternative<NBT::cList>(m_Content));

	auto & List = std::get<NBT::cList>(m_Content);
	VERIFY(
		List.empty() ||
		List.back().m_Content.index() == a_NewContent.index()
	);

	List.emplace_back(std::move(a_NewContent));
}





NBT::cNBTContent cNBT::Pop(long a_Index)
{
	VERIFY(std::holds_alternative<NBT::cList>(m_Content));
	auto & List = std::get<NBT::cList>(m_Content);
	return List.erase(List.begin() + a_Index)->Expose();
}





NBT::cNBTContent cNBT::Pop(NBT::cList::iterator a_Index)
{
	VERIFY(std::holds_alternative<NBT::cList>(m_Content));
	auto & List = std::get<NBT::cList>(m_Content);
	return List.erase(a_Index)->Expose();
}





void cNBT::Serialize(const cNBT & a_Compound, cFastNBTWriter & a_Writer)
{
	a_Writer.BeginCompound("");
	VERIFY(std::holds_alternative<NBT::cCompound *>(a_Compound.Expose()));
	const auto Compound = a_Compound.ExposeCompound();
	for (const auto & Entry : Compound)
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
		Compound.ExposeCompound()[a_NBT.GetName(CurrentLine)] = {Type, DeserializeEntry(a_NBT, CurrentLine, Type)};
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
			a_Writer.AddByte(a_Name, std::get<unsigned char>(a_Entry.Expose()));
			break;
		}
		case TAG_Short:
		{
			a_Writer.AddShort(a_Name, std::get<Int16>(a_Entry.Expose()));
			break;
		}
		case TAG_Int:
		{
			a_Writer.AddInt(a_Name, std::get<Int32>(a_Entry.Expose()));
			break;
		}
		case TAG_Long:
		{
			a_Writer.AddLong(a_Name, std::get<Int64>(a_Entry.Expose()));
			break;
		}
		case TAG_Float:
		{
			a_Writer.AddFloat(a_Name, std::get<float>(a_Entry.Expose()));
			break;
		}
		case TAG_Double:
		{
			a_Writer.AddDouble(a_Name, std::get<double>(a_Entry.Expose()));
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
			const auto GlobalType =  Array.begin()->Expose().index();
			a_Writer.BeginList(a_Name, static_cast<eTagType>(GlobalType));
			for (const auto & ArrayEntry : Array)
			{
				VERIFY(ArrayEntry.Expose().index() == GlobalType);
				auto Type = static_cast<eTagType>(ArrayEntry.Expose().index());
				SerializeEntry("", ArrayEntry.Expose(), Type, a_Writer);
			}
			a_Writer.EndList();
			break;
		}
		case TAG_Compound:
		{
			Serialize(a_Entry, a_Writer);
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
			VERIFY(a_NBT.GetChildrenType(a_TagIdx) == TAG_Byte);
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
			VERIFY(a_NBT.GetChildrenType(a_TagIdx) == TAG_Int);
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
