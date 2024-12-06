
// ChunkData.h

// Declares the cChunkData class that represents the block's type, meta, blocklight and skylight storage for a chunk





#pragma once

#include "FunctionRef.h"
#include "BlockType.h"





template <class ElementType, size_t ElementCount>
struct ChunkDataStore
{
	using Type = std::array<ElementType, ElementCount>;

	ChunkDataStore(ElementType a_DefaultValue) : DefaultValue(a_DefaultValue) {}

	/** Copy assign from another ChunkDataStore. */
	void Assign(const ChunkDataStore<ElementType, ElementCount> & a_Other);

	/** Gets one value at the given position.
	Returns DefaultValue if the section is not allocated. */
	ElementType Get(Vector3i a_Position) const;

	/** Returns a raw pointer to the internal representation of the specified section.
	Will be nullptr if the section is not allocated. */
	Type * GetSection(size_t a_Y) const;

	/** Sets one value at the given position.
	Allocates a section if needed for the operation. */
	void Set(Vector3i a_Position, ElementType a_Value);

	/** Copies the data from the specified flat section array into the internal representation.
	Allocates a section if needed for the operation. */
	void SetSection(const ElementType (& a_Source)[ElementCount], size_t a_Y);

	/** Copies the data from the specified flat array into the internal representation.
	Allocates sections that are needed for the operation. */
	void SetAll(const ElementType (& a_Source)[cChunkDef::NumSections * ElementCount]);

	/** Contains all the sections this ChunkDataStore manages. */
	std::unique_ptr<Type> Store[cChunkDef::NumSections];
	ElementType DefaultValue;
};





class ChunkBlockData
{
public:

	ChunkBlockData() : m_Blocks(DefaultValue) {}

	static constexpr size_t SectionBlockCount = cChunkDef::SectionHeight * cChunkDef::Width * cChunkDef::Width;
	static constexpr size_t SectionMetaCount = SectionBlockCount / 2;

	static constexpr BlockState DefaultValue = Block::Air::Air();

	using SectionType = BlockState[SectionBlockCount];
	using SectionMetaType = unsigned char[SectionMetaCount];

private:

	ChunkDataStore<BlockState, SectionBlockCount> m_Blocks;

public:

	using BlockArray = decltype(m_Blocks)::Type;

	void Assign(const ChunkBlockData & a_Other);

	BlockState GetBlock(Vector3i a_Position) const { return m_Blocks.Get(a_Position); }

	BlockArray * GetSection(size_t a_Y) const { return m_Blocks.GetSection(a_Y); }

	void SetBlock(Vector3i a_Position, BlockState a_Block) { m_Blocks.Set(a_Position, a_Block); }

	void SetAll(const cChunkDef::BlockStates & a_BlockSource);
	void SetSection(const SectionType & a_BlockSource, size_t a_Y);
};





class ChunkLightData
{
public:

	ChunkLightData() : m_BlockLights(DefaultBlockLightValue), m_SkyLights(DefaultBlockLightValue) {}

	static constexpr size_t SectionLightCount = (cChunkDef::SectionHeight * cChunkDef::Width * cChunkDef::Width) / 2;

	static constexpr LIGHTTYPE DefaultBlockLightValue = 0x00;
	static constexpr LIGHTTYPE DefaultSkyLightValue = 0xFF;

	using SectionType = LIGHTTYPE[SectionLightCount];

private:

	ChunkDataStore<LIGHTTYPE, SectionLightCount> m_BlockLights;
	ChunkDataStore<LIGHTTYPE, SectionLightCount> m_SkyLights;

public:

	using LightArray = decltype(m_BlockLights)::Type;

	void Assign(const ChunkLightData & a_Other);

	LIGHTTYPE GetBlockLight(Vector3i a_Position) const { return m_BlockLights.Get(a_Position); }
	LIGHTTYPE GetSkyLight(Vector3i a_Position) const { return m_SkyLights.Get(a_Position); }

	LightArray * GetBlockLightSection(size_t a_Y) const { return m_BlockLights.GetSection(a_Y); }
	LightArray * GetSkyLightSection(size_t a_Y) const { return m_SkyLights.GetSection(a_Y); }

	void SetAll(const cChunkDef::LightNibbles & a_BlockLightSource, const cChunkDef::LightNibbles & a_SkyLightSource);
	void SetSection(const SectionType & a_BlockLightSource, const SectionType & a_SkyLightSource, size_t a_Y);
};





/** Invokes the callback functor for every chunk section containing at least one present block or light section data.
This is used to collect all data for all sections.
In macro form to work around a Visual Studio 2017 ICE bug. */
#define ChunkDef_ForEachSection(BlockData, LightData, Callback) \
	do \
	{ \
		for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y) \
		{ \
			const auto Blocks = BlockData.GetSection(Y); \
			const auto BlockLights = LightData.GetBlockLightSection(Y); \
			const auto SkyLights = LightData.GetSkyLightSection(Y); \
			if ((Blocks != nullptr) || (BlockLights != nullptr) || (SkyLights != nullptr)) \
			{ \
				Callback \
			} \
		} \
	} while (false)





extern template struct ChunkDataStore<BlockState, ChunkBlockData::SectionBlockCount>;
extern template struct ChunkDataStore<LIGHTTYPE, ChunkLightData::SectionLightCount>;
