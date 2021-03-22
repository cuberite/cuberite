
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
			static_cast<size_t>(cChunkDef::MakeIndex(a_RelPos.x, a_RelPos.y % cChunkDef::SectionHeight, a_RelPos.z))
		};
	}

	bool IsCompressed(const size_t ElementCount)
	{
		return ElementCount != ChunkBlockData::SectionBlockCount;
	}

	template <size_t ElementCount, typename ValueType>
	ValueType UnpackDefaultValue(const ValueType m_DefaultValue)
	{
		if (IsCompressed(ElementCount))
		{
			return m_DefaultValue;
		}

		return m_DefaultValue;
	}
}  // namespace (anonymous)





template<class ElementType, size_t ElementCount>
void ChunkDataStore<ElementType, ElementCount>::Assign(const ChunkDataStore<ElementType, ElementCount> & a_Other)
{
	m_DefaultValue = a_Other.m_DefaultValue;

	for (size_t Y = 0; Y != cChunkDef::NumSections; Y++)
	{
		Store[Y].reset();

		if (const auto & Other = a_Other.Store[Y]; Other != nullptr)
		{
			Store[Y] = std::make_unique<Type>(*Other);
		}
	}
}





template<class ElementType, size_t ElementCount>
ElementType ChunkDataStore<ElementType, ElementCount>::Get(const Vector3i a_Position) const
{
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

	return UnpackDefaultValue<ElementCount>(m_DefaultValue);
}





template<class ElementType, size_t ElementCount>
void ChunkDataStore<ElementType, ElementCount>::Set(const Vector3i a_Position, const ElementType a_Value)
{
	const auto Indices = IndicesFromRelPos(a_Position);
	auto & Section = Store[Indices.Section];

	if (Section == nullptr)
	{
		if (a_Value == UnpackDefaultValue<ElementCount>(m_DefaultValue))
		{
			return;
		}

		Section = cpp20::make_unique_for_overwrite<Type>();
		std::fill(Section->begin(), Section->end(), m_DefaultValue);
	}
	(*Section)[Indices.Index] = a_Value;
}





template<class ElementType, size_t ElementCount>
void ChunkDataStore<ElementType, ElementCount>::SetSection(const ElementType (& a_Source)[ElementCount], const size_t a_Y)
{
	auto & Section = Store[a_Y];
	const auto SourceEnd = std::end(a_Source);

	if (Section != nullptr)
	{
		std::copy(a_Source, SourceEnd, Section->begin());
	}
	else if (std::any_of(a_Source, SourceEnd, [this](const auto Value) { return Value != m_DefaultValue; }))
	{
		Section = cpp20::make_unique_for_overwrite<Type>();
		std::copy(a_Source, SourceEnd, Section->begin());
	}
}





template<class ElementType, size_t ElementCount>
void ChunkDataStore<ElementType, ElementCount>::SetAll(const ElementType (& a_Source)[cChunkDef::NumSections * ElementCount])
{
	for (size_t Y = 0; Y != cChunkDef::NumSections; Y++)
	{
		SetSection(*reinterpret_cast<const ElementType (*)[ElementCount]>(a_Source + Y * ElementCount), Y);
	}
}





void ChunkBlockData::Assign(const ChunkBlockData & a_Other)
{
	for (size_t Y = 0; Y != cChunkDef::NumSections; Y++)
	{
		m_Blocks[Y].reset();

		if (const auto & Other = a_Other.m_Blocks[Y]; Other != nullptr)
		{
			m_Blocks.at(Y) = std::make_unique<std::array<BlockState, SectionBlockCount>>(*Other);
		}
	}
}





BlockState ChunkBlockData::GetBlock(Vector3i a_Position) const
{
	const auto Indices = IndicesFromRelPos(a_Position);
	const auto & Section = m_Blocks[Indices.Section];

	if (Section != nullptr)
	{
		return (*Section)[Indices.Index];
	}

	return UnpackDefaultValue<SectionBlockCount>(DefaultValue);
}





const ChunkBlockData::SectionType & ChunkBlockData::GetSection(size_t a_Y) const
{
	return m_Blocks.at(a_Y);
}





void ChunkBlockData::SetBlock(Vector3i a_Position, BlockState a_Block)
{
	const auto Indices = IndicesFromRelPos(a_Position);
	auto & Section = m_Blocks[Indices.Section];

	if (Section == nullptr)
	{
		if (a_Block == UnpackDefaultValue<SectionBlockCount>(DefaultValue))
		{
			return;
		}

		Section = cpp20::make_unique_for_overwrite<std::array<BlockState, SectionBlockCount>>();
		std::fill(Section->begin(), Section->end(), DefaultValue);
	}
	(*Section)[Indices.Index] = a_Block;
}





void ChunkBlockData::SetAll(const cChunkDef::BlockStates & a_BlockSource)
{
	for (size_t Y = 0; Y != cChunkDef::NumSections; Y++)
	{
		auto Section = std::make_unique<std::array<BlockState, SectionBlockCount>>();
		std::memmove(Section->data(), a_BlockSource.data() + (Y * SectionBlockCount), SectionBlockCount);
		SetSection(std::move(Section), Y);
	}
}





void ChunkBlockData::SetSection(const SectionType & a_BlockSource, const size_t a_Y)
{
	auto & Section = m_Blocks[a_Y];

	if (Section != nullptr)
	{
		std::copy(a_BlockSource->begin(), a_BlockSource->end(), Section->begin());
		return;
	}
	/*
	else if (std::any_of(a_BlockSource->begin(), a_BlockSource->end(), [&](const auto Value) { return Value != Block::Air::Air(); }))
	{
	*/
		Section = cpp20::make_unique_for_overwrite<std::array<BlockState, SectionBlockCount>>();
		std::copy(a_BlockSource->begin(), a_BlockSource->end(), Section->begin());
	// }  // TODO(12xx12)
}





void ChunkLightData::Assign(const ChunkLightData & a_Other)
{
	m_BlockLights.Assign(a_Other.m_BlockLights);
	m_SkyLights.Assign(a_Other.m_SkyLights);
}





void ChunkLightData::SetAll(const cChunkDef::LightNibbles & a_BlockLightSource, const cChunkDef::LightNibbles & a_SkyLightSource)
{
	m_BlockLights.SetAll(a_BlockLightSource);
	m_SkyLights.SetAll(a_SkyLightSource);
}





void ChunkLightData::SetSection(const SectionType & a_BlockLightSource, const SectionType & a_SkyLightSource, const size_t a_Y)
{
	m_BlockLights.SetSection(a_BlockLightSource, a_Y);
	m_SkyLights.SetSection(a_SkyLightSource, a_Y);
}





template struct ChunkDataStore<LIGHTTYPE, ChunkLightData::SectionLightCount>;

