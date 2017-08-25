
// SetChunkData.h

// Declares the cSetChunkData class used for sending loaded / generated chunk data into cWorld





#pragma once





class cSetChunkData
{
public:
	/** Constructs a new instance with empty data.
	Allocates new buffers for the block data.
	Prefer to use this constructor, then fill the object with data and then send it to cWorld, as this will
	reduce the copying required to queue the set operation. */
	cSetChunkData(int a_ChunkX, int a_ChunkZ, bool a_ShouldMarkDirty);

	/** Constructs a new instance based on data existing elsewhere, will copy all the memory. Prefer to use the
	other constructor as much as possible.
	Will move the entity list and blockentities into the internal storage, and invalidate a_Entities and
	a_BlockEntities.
	When passing an lvalue, a_Entities and a_BlockEntities must be explicitly converted to an rvalue beforehand
	with std::move().
	a_BlockTypes and a_BlockMetas must always be valid.
	If either of the light arrays are nullptr, the chunk data will be marked as not having any light at all and
	will be scheduled for re-lighting once it is set into the chunkmap.
	If a_Biomes is not valid, the internal flag is set and the world will calculate the biomes using the chunk
	generator when setting the chunk data.
	If a_HeightMap is not assigned, the world will calculate the heightmap based on the blocktypes when setting
	the chunk data. */
	cSetChunkData(
		int a_ChunkX, int a_ChunkZ,
		const BLOCKTYPE * a_BlockTypes,
		const NIBBLETYPE * a_BlockMetas,
		const NIBBLETYPE * a_BlockLight,
		const NIBBLETYPE * a_SkyLight,
		const cChunkDef::HeightMap * a_HeightMap,
		const cChunkDef::BiomeMap * a_Biomes,
		cEntityList && a_Entities,
		cBlockEntities && a_BlockEntities,
		bool a_ShouldMarkDirty
	);

	int GetChunkX(void) const { return m_ChunkX; }
	int GetChunkZ(void) const { return m_ChunkZ; }

	/** Returns the internal storage of the block types, read-only. */
	const cChunkDef::BlockTypes & GetBlockTypes(void) const { return m_BlockTypes; }

	/** Returns the internal storage of the block types, read-only. */
	const cChunkDef::BlockNibbles & GetBlockMetas(void) const { return m_BlockMetas; }

	/** Returns the internal storage of the block light, read-only. */
	const cChunkDef::BlockNibbles & GetBlockLight(void) const { return m_BlockLight; }

	/** Returns the internal storage of the block types, read-only. */
	const cChunkDef::BlockNibbles & GetSkyLight(void) const { return m_SkyLight; }

	/** Returns the internal storage for heightmap, read-only. */
	const cChunkDef::HeightMap & GetHeightMap(void) const { return m_HeightMap; }

	/** Returns the internal storage for biomes, read-write. */
	cChunkDef::BiomeMap & GetBiomes(void) { return m_Biomes; }

	/** Returns the internal storage for entities, read-write. */
	cEntityList & GetEntities(void) { return m_Entities; }

	/** Returns the internal storage for block entities, read-write. */
	cBlockEntities & GetBlockEntities(void) { return m_BlockEntities; }

	/** Returns whether both light arrays stored in this object are valid. */
	bool IsLightValid(void) const { return m_IsLightValid; }

	/** Returns whether the heightmap stored in this object is valid. */
	bool IsHeightMapValid(void) const { return m_IsHeightMapValid; }

	/** Returns whether the biomes stored in this object are valid. */
	bool AreBiomesValid(void) const { return m_AreBiomesValid; }

	/** Returns whether the chunk should be marked as dirty after its data is set.
	Used by the generator to save chunks after generating. */
	bool ShouldMarkDirty(void) const { return m_ShouldMarkDirty; }

	/** Marks the biomes stored in this object as valid. */
	void MarkBiomesValid(void) { m_AreBiomesValid = true; }

	/** Calculates the heightmap based on the contained blocktypes and marks it valid. */
	void CalculateHeightMap(void);

	/** Removes the block entities that don't have a proper blocktype at their corresponding coords. */
	void RemoveInvalidBlockEntities(void);

protected:
	int m_ChunkX;
	int m_ChunkZ;

	cChunkDef::BlockTypes m_BlockTypes;
	cChunkDef::BlockNibbles m_BlockMetas;
	cChunkDef::BlockNibbles m_BlockLight;
	cChunkDef::BlockNibbles m_SkyLight;
	cChunkDef::HeightMap m_HeightMap;
	cChunkDef::BiomeMap m_Biomes;
	cEntityList m_Entities;
	cBlockEntities m_BlockEntities;

	bool m_IsLightValid;
	bool m_IsHeightMapValid;
	bool m_AreBiomesValid;
	bool m_ShouldMarkDirty;
};
