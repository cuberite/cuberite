
// ChunkData.cpp

// Implements the cChunkData class that represents the block's type, meta, blocklight and skylight storage for a chunk

#include "Globals.h"
#include "ChunkData.h"
#include "BlockType.h"





namespace
{
	struct SectionIndices
	{
		size_t Section = 0;  // Index into m_Sections
		size_t Index = 0;    // Index into a single sChunkSection
	};

	inline SectionIndices IndicesFromRelPos(const Vector3i a_RelPos)
	{
		ASSERT(cChunkDef::IsValidRelPos(a_RelPos));

		return
		{
			static_cast<size_t>(a_RelPos.y / cChunkDef::SectionHeight),
			cChunkDef::MakeIndex(a_RelPos.x, a_RelPos.y % cChunkDef::SectionHeight, a_RelPos.z)
		};
	}

	bool IsCompressed(const size_t ElementCount)
	{
		return ElementCount != ChunkBlockData::SectionBlockCount;
	}

	template <size_t ElementCount, typename ValueType>
	ValueType UnpackDefaultValue(const ValueType DefaultValue)
	{
		if (IsCompressed(ElementCount))
		{
			return DefaultValue & 0xF;
		}

		return DefaultValue;
	}
}  // namespace (anonymous)





template<class ElementType, size_t ElementCount, ElementType DefaultValue>
void ChunkDataStore<ElementType, ElementCount, DefaultValue>::Assign(const ChunkDataStore<ElementType, ElementCount, DefaultValue> & a_Other)
{
	for (size_t Y = 0; Y != cChunkDef::NumSections; Y++)
	{
		Store[Y].reset();

		if (const auto & Other = a_Other.Store[Y]; Other != nullptr)
		{
			Store[Y] = std::make_unique<Type>(*Other);
		}
	}
}





template<class ElementType, size_t ElementCount, ElementType DefaultValue>
ElementType ChunkDataStore<ElementType, ElementCount, DefaultValue>::Get(const Vector3i a_Position) const
{
    if (!cChunkDef::IsValidRelPos(a_Position))
    {
        ASSERT(false);
        UnpackDefaultValue<ElementCount>(DefaultValue);
    }

	const auto Indices = IndicesFromRelPos(a_Position);
	const auto & Section = Store[Indices.Section];

	if (Section != nullptr)
	{
		if (IsCompressed(ElementCount))
		{
			return cChunkDef::ExpandNibble(Section->data(), Indices.Index);
		}
		else
		{
			return (*Section)[Indices.Index];
		}
	}

	return UnpackDefaultValue<ElementCount>(DefaultValue);
}





template<class ElementType, size_t ElementCount, ElementType DefaultValue>
typename ChunkDataStore<ElementType, ElementCount, DefaultValue>::Type * ChunkDataStore<ElementType, ElementCount, DefaultValue>::GetSection(const size_t a_Y) const
{
    if (a_Y >= cChunkDef::NumSections)
    {
        return nullptr;
    }

	return Store[a_Y].get();
}





template<class ElementType, size_t ElementCount, ElementType DefaultValue>
void ChunkDataStore<ElementType, ElementCount, DefaultValue>::Set(const Vector3i a_Position, const ElementType a_Value)
{
    if (!cChunkDef::IsValidRelPos(a_Position))
    {
        return;
    }

	const auto Indices = IndicesFromRelPos(a_Position);
	auto & Section = Store[Indices.Section];

	if (Section == nullptr)
	{
		if (a_Value == UnpackDefaultValue<ElementCount>(DefaultValue))
		{
			return;
		}

		Section = cpp20::make_unique_for_overwrite<Type>();
		std::fill(Section->begin(), Section->end(), DefaultValue);
	}

	if (IsCompressed(ElementCount))
	{
		cChunkDef::PackNibble(Section->data(), Indices.Index, a_Value);
	}
	else
	{
		(*Section)[Indices.Index] = a_Value;
	}
}





template<class ElementType, size_t ElementCount, ElementType DefaultValue>
void ChunkDataStore<ElementType, ElementCount, DefaultValue>::SetSection(const ElementType (& a_Source)[ElementCount], const size_t a_Y)
{
    if (a_Y >= cChunkDef::NumSections)
    {
        return;
    }

	auto & Section = Store[a_Y];
	const auto SourceEnd = std::end(a_Source);

	if (Section != nullptr)
	{
		std::copy(a_Source, SourceEnd, Section->begin());
	}
	else if (std::any_of(a_Source, SourceEnd, [](const auto Value) { return Value != DefaultValue; }))
	{
		Section = cpp20::make_unique_for_overwrite<Type>();
		std::copy(a_Source, SourceEnd, Section->begin());
	}
}





template<class ElementType, size_t ElementCount, ElementType DefaultValue>
void ChunkDataStore<ElementType, ElementCount, DefaultValue>::SetAll(const ElementType (& a_Source)[cChunkDef::NumSections * ElementCount])
{
	for (size_t Y = 0; Y != cChunkDef::NumSections; Y++)
	{
		SetSection(*reinterpret_cast<const ElementType (*)[ElementCount]>(a_Source + Y * ElementCount), Y);
	}
}





void ChunkBlockData::Assign(const ChunkBlockData & a_Other)
{
	m_Blocks.Assign(a_Other.m_Blocks);
	m_Metas.Assign(a_Other.m_Metas);
}





void ChunkBlockData::SetAll(const cChunkDef::BlockTypes & a_BlockSource, const cChunkDef::BlockNibbles & a_MetaSource)
{
	m_Blocks.SetAll(a_BlockSource);
	m_Metas.SetAll(a_MetaSource);
}





void ChunkBlockData::SetSection(const SectionType & a_BlockSource, const SectionMetaType & a_MetaSource, const size_t a_Y)
{
	m_Blocks.SetSection(a_BlockSource, a_Y);
	m_Metas.SetSection(a_MetaSource, a_Y);
}





void ChunkLightData::Assign(const ChunkLightData & a_Other)
{
	m_BlockLights.Assign(a_Other.m_BlockLights);
	m_SkyLights.Assign(a_Other.m_SkyLights);
}





void ChunkLightData::SetAll(const cChunkDef::BlockNibbles & a_BlockLightSource, const cChunkDef::BlockNibbles & a_SkyLightSource)
{
	m_BlockLights.SetAll(a_BlockLightSource);
	m_SkyLights.SetAll(a_SkyLightSource);
}





void ChunkLightData::SetSection(const SectionType & a_BlockLightSource, const SectionType & a_SkyLightSource, const size_t a_Y)
{
	m_BlockLights.SetSection(a_BlockLightSource, a_Y);
	m_SkyLights.SetSection(a_SkyLightSource, a_Y);
}





template struct ChunkDataStore<BLOCKTYPE, ChunkBlockData::SectionBlockCount, ChunkBlockData::DefaultValue>;
template struct ChunkDataStore<NIBBLETYPE, ChunkBlockData::SectionMetaCount, ChunkLightData::DefaultBlockLightValue>;
template struct ChunkDataStore<NIBBLETYPE, ChunkLightData::SectionLightCount, ChunkLightData::DefaultSkyLightValue>;
