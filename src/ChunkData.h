
// ChunkData.h

// Declares the cChunkData class that represents the block's type, meta, blocklight and skylight storage for a chunk





#pragma once

#include "FunctionRef.h"
#include "ChunkDef.h"
#include "PointOfInterest.h"





template <class ElementType, size_t ElementCount, ElementType DefaultValue>
struct ChunkDataStore
{
	using Type = std::array<ElementType, ElementCount>;

	/** Copy assign from another ChunkDataStore. */
	void Assign(const ChunkDataStore<ElementType, ElementCount, DefaultValue> & a_Other);

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
};





class ChunkBlockData
{
public:

	static constexpr size_t SectionBlockCount = cChunkDef::SectionHeight * cChunkDef::Width * cChunkDef::Width;
	static constexpr size_t SectionMetaCount = SectionBlockCount / 2;

	static constexpr BLOCKTYPE DefaultValue = 0x00;
	static constexpr NIBBLETYPE DefaultMetaValue = 0x00;

	using SectionType = BLOCKTYPE[SectionBlockCount];
	using SectionMetaType = NIBBLETYPE[SectionMetaCount];

private:

	ChunkDataStore<BLOCKTYPE, SectionBlockCount, DefaultValue> m_Blocks;
	ChunkDataStore<NIBBLETYPE, SectionMetaCount, DefaultMetaValue> m_Metas;

public:

	using BlockArray = decltype(m_Blocks)::Type;
	using MetaArray = decltype(m_Metas)::Type;

	void Assign(const ChunkBlockData & a_Other);

	BLOCKTYPE GetBlock(Vector3i a_Position) const { return m_Blocks.Get(a_Position); }
	NIBBLETYPE GetMeta(Vector3i a_Position) const { return m_Metas.Get(a_Position); }

	BlockArray * GetSection(size_t a_Y) const { return m_Blocks.GetSection(a_Y); }
	MetaArray * GetMetaSection(size_t a_Y) const { return m_Metas.GetSection(a_Y); }

	void SetBlock(Vector3i a_Position, BLOCKTYPE a_Block) { m_Blocks.Set(a_Position, a_Block); }
	void SetMeta(Vector3i a_Position, NIBBLETYPE a_Meta) { m_Metas.Set(a_Position, a_Meta); }

	void SetAll(const cChunkDef::BlockTypes & a_BlockSource, const cChunkDef::BlockNibbles & a_MetaSource);
	void SetSection(const SectionType & a_BlockSource, const SectionMetaType & a_MetaSource, size_t a_Y);
};





class ChunkLightData
{
public:

	static constexpr size_t SectionLightCount = (cChunkDef::SectionHeight * cChunkDef::Width * cChunkDef::Width) / 2;

	static constexpr NIBBLETYPE DefaultBlockLightValue = 0x00;
	static constexpr NIBBLETYPE DefaultSkyLightValue = 0xFF;

	using SectionType = NIBBLETYPE[SectionLightCount];

private:

	ChunkDataStore<NIBBLETYPE, SectionLightCount, DefaultBlockLightValue> m_BlockLights;
	ChunkDataStore<NIBBLETYPE, SectionLightCount, DefaultSkyLightValue> m_SkyLights;

public:

	using LightArray = decltype(m_BlockLights)::Type;

	void Assign(const ChunkLightData & a_Other);

	NIBBLETYPE GetBlockLight(Vector3i a_Position) const { return m_BlockLights.Get(a_Position); }
	NIBBLETYPE GetSkyLight(Vector3i a_Position) const { return m_SkyLights.Get(a_Position); }

	LightArray * GetBlockLightSection(size_t a_Y) const { return m_BlockLights.GetSection(a_Y); }
	LightArray * GetSkyLightSection(size_t a_Y) const { return m_SkyLights.GetSection(a_Y); }

	void SetAll(const cChunkDef::BlockNibbles & a_BlockLightSource, const cChunkDef::BlockNibbles & a_SkyLightSource);
	void SetSection(const SectionType & a_BlockLightSource, const SectionType & a_SkyLightSource, size_t a_Y);
};





class cChunkPoiData
{
public:
	typedef std::array<std::vector<cPointOfInterest>, cChunkDef::SectionHeight> PoiData;
	typedef PoiData::value_type PoiArray;

private:
	PoiData m_Poies;

public:

	void Assign(const cChunkPoiData & a_Other);

	void AddPoi(const cPointOfInterest & a_Poi);
	// Only removes POI if it exists
	void RemovePoi(Vector3i a_Position);

	const PoiArray & GetPoies(size_t a_Y) const { return m_Poies.at(a_Y); }

	PoiData & Data() { return m_Poies; }
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
			const auto Metas = BlockData.GetMetaSection(Y); \
			const auto BlockLights = LightData.GetBlockLightSection(Y); \
			const auto SkyLights = LightData.GetSkyLightSection(Y); \
			if ((Blocks != nullptr) || (Metas != nullptr) || (BlockLights != nullptr) || (SkyLights != nullptr)) \
			{ \
				Callback \
			} \
		} \
	} while (false)





extern template struct ChunkDataStore<BLOCKTYPE, ChunkBlockData::SectionBlockCount, ChunkBlockData::DefaultValue>;
extern template struct ChunkDataStore<NIBBLETYPE, ChunkBlockData::SectionMetaCount, ChunkLightData::DefaultBlockLightValue>;
extern template struct ChunkDataStore<NIBBLETYPE, ChunkLightData::SectionLightCount, ChunkLightData::DefaultSkyLightValue>;
