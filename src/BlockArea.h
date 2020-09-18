
// BlockArea.h

// Interfaces to the cBlockArea object representing an area of block data that can be queried from cWorld and then accessed again without further queries
// The object also supports writing the blockdata back into cWorld, even into other coords

// NOTE: All Nibble values (meta, blocklight, skylight) are stored one-nibble-per-byte for faster access / editting!
// NOTE: Lua bindings for this object explicitly check parameter values. C++ code is expected to pass in valid params, so the functions ASSERT on invalid params.
//    This includes the datatypes (must be present / valid combination), coords and sizes.





#pragma once

#include "BlockType.h"
#include "ForEachChunkProvider.h"
#include "ChunkDataCallback.h"
#include "Cuboid.h"
#include "FunctionRef.h"
#include "BlockEntities/BlockEntity.h"




// fwd:
class cCuboid;
class cItem;
class cItems;
using cBlockEntityCallback = cFunctionRef<bool(cBlockEntity &)>;




// tolua_begin
class cBlockArea
{
	// tolua_end
	DISALLOW_COPY_AND_ASSIGN(cBlockArea);
	// tolua_begin

public:

	/** What data is to be queried (bit-mask) */
	enum
	{
		baTypes         =  1,
		baMetas         =  2,
		baLight         =  4,
		baSkyLight      =  8,
		// baEntities   = 16,  // Not supported yet
		baBlockEntities = 32,
	} ;

	/** The per-block strategy to use when merging another block area into this object.
	See the Merge function for the description of these */
	enum eMergeStrategy
	{
		msOverwrite,
		msFillAir,
		msImprint,
		msLake,
		msSpongePrint,
		msDifference,
		msSimpleCompare,
		msMask,
	} ;

	cBlockArea(void);

	/** Returns true if the datatype combination is valid.
	Invalid combinations include BlockEntities without BlockTypes. */
	static bool IsValidDataTypeCombination(int a_DataTypes);

	/** Clears the data stored to reclaim memory */
	void Clear(void);

	// tolua_end

	/** Creates a new area of the specified size and contents.
	Origin is set to all zeroes.
	BlockTypes are set to air, block metas to zero, blocklights to zero and skylights to full light. */
	void Create(int a_SizeX, int a_SizeY, int a_SizeZ, int a_DataTypes = baTypes | baMetas | baBlockEntities);

	/** Creates a new area of the specified size and contents.
	Origin is set to all zeroes.
	BlockTypes are set to air, block metas to zero, blocklights to zero and skylights to full light. */
	void Create(const Vector3i & a_Size, int a_DataTypes = baTypes | baMetas | baBlockEntities);

	// tolua_begin

	/** Resets the origin. No other changes are made, contents are untouched. */
	void SetOrigin(int a_OriginX, int a_OriginY, int a_OriginZ);

	/** Resets the origin. No other changes are made, contents are untouched. */
	void SetOrigin(const Vector3i & a_Origin);

	/** Returns true if the specified relative coords are within this area's coord range (0 - m_Size). */
	bool IsValidRelCoords(int a_RelX, int a_RelY, int a_RelZ) const;

	/** Returns true if the specified relative coords are within this area's coord range (0 - m_Size). */
	bool IsValidRelCoords(const Vector3i & a_RelCoords) const;

	/** Returns true if the specified coords are within this area's coord range (as indicated by m_Origin). */
	bool IsValidCoords(int a_BlockX, int a_BlockY, int a_BlockZ) const;

	/** Returns true if the specified coords are within this area's coord range (as indicated by m_Origin). */
	bool IsValidCoords(const Vector3i & a_Coords) const;

	// tolua_end

	/** Reads an area of blocks specified. Returns true if successful. All coords are inclusive. */
	bool Read(cForEachChunkProvider & a_ForEachChunkProvider, int a_MinBlockX, int a_MaxBlockX, int a_MinBlockY, int a_MaxBlockY, int a_MinBlockZ, int a_MaxBlockZ, int a_DataTypes = baTypes | baMetas | baBlockEntities);

	/** Reads an area of blocks specified. Returns true if successful. The bounds are included in the read area. */
	bool Read(cForEachChunkProvider & a_ForEachChunkProvider, const cCuboid & a_Bounds, int a_DataTypes = baTypes | baMetas | baBlockEntities);

	/** Reads an area of blocks specified. Returns true if successful. The bounds are included in the read area. */
	bool Read(cForEachChunkProvider & a_ForEachChunkProvider, const Vector3i & a_Point1, const Vector3i & a_Point2, int a_DataTypes = baTypes | baMetas | baBlockEntities);

	// TODO: Write() is not too good an interface: if it fails, there's no way to repeat only for the parts that didn't write
	// A better way may be to return a list of cBlockAreas for each part that didn't succeed writing, so that the caller may try again

	/** Writes the area back into cWorld at the coords specified. Returns true if successful in all chunks, false if only partially / not at all.
	Doesn't wake up the simulators. */
	bool Write(cForEachChunkProvider & a_ForEachChunkProvider, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes);

	/** Writes the area back into cWorld at the coords specified. Returns true if successful in all chunks, false if only partially / not at all.
	Doesn't wake up the simulators. */
	bool Write(cForEachChunkProvider & a_ForEachChunkProvider, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ)
	{
		// Write all available data
		return Write(a_ForEachChunkProvider, a_MinBlockX, a_MinBlockY, a_MinBlockZ, GetDataTypes());
	}

	/** Writes the area back into cWorld at the coords specified. Returns true if successful in all chunks, false if only partially / not at all.
	Doesn't wake up the simulators. */
	bool Write(cForEachChunkProvider & a_ForEachChunkProvider, const Vector3i & a_MinCoords, int a_DataTypes);

	/** Writes the area back into cWorld at the coords specified. Returns true if successful in all chunks, false if only partially / not at all.
	Doesn't wake up the simulators. */
	bool Write(cForEachChunkProvider & a_ForEachChunkProvider, const Vector3i & a_MinCoords)
	{
		// Write all available data
		return Write(a_ForEachChunkProvider, a_MinCoords.x, a_MinCoords.y, a_MinCoords.z, GetDataTypes());
	}

	// tolua_begin

	/** Copies this object's contents into the specified BlockArea. */
	void CopyTo(cBlockArea & a_Into) const;

	/** Copies the contents from the specified BlockArea into this object. */
	void CopyFrom(const cBlockArea & a_From);

	/** For testing purposes only, dumps the area into a file. */
	void DumpToRawFile(const AString & a_FileName);

	/** Crops the internal contents by the specified amount of blocks from each border. */
	void Crop(int a_AddMinX, int a_SubMaxX, int a_AddMinY, int a_SubMaxY, int a_AddMinZ, int a_SubMaxZ);

	/** Expands the internal contents by the specified amount of blocks from each border */
	void Expand(int a_SubMinX, int a_AddMaxX, int a_SubMinY, int a_AddMaxY, int a_SubMinZ, int a_AddMaxZ);

	/** Merges another block area into this one, using the specified block combinating strategy
	This function combines another BlockArea into the current object.
	The a_RelX, a_RelY and a_RelZ parameters specify the coords of this BA where a_Src should be copied.
	If both areas contain baBlockEntities, the BEs are merged (with preference of keeping this' ones) (MergeBlockEntities()).
	If only this contains BEs, but a_Src doesn't, the BEs are checked after merge to remove the overwritten ones and create
	the missing ones (UpdateBlockEntities()).
	The strategy parameter specifies how individual blocks are combined together, using the table below.

	| area block |                 result              |
	| this | Src | msOverwrite | msFillAir | msImprint |
	+------+-----+-------------+-----------+-----------+
	| air  | air | air         | air       | air       |
	| A    | air | air         | A         | A         |
	| air  | B   | B           | B         | B         |
	| A    | B   | B           | A         | B         |

	So to sum up:
	- msOverwrite completely overwrites all blocks with the Src's blocks
	- msFillAir overwrites only those blocks that were air
	- msImprint overwrites with only those blocks that are non-air

	Special strategies (evaluate top-down, first match wins):
	msLake:
	|    area block     |        |
	|   this   | Src    | result |
	+----------+--------+--------+
	| A        | sponge | A      |  Sponge is the NOP block
	| *        | air    | air    |  Air always gets hollowed out, even under the oceans
	| water    | *      | water  |  Water is never overwritten
	| lava     | *      | lava   |  Lava is never overwritten
	| *        | water  | water  |  Water always overwrites anything
	| *        | lava   | lava   |  Lava always overwrites anything
	| dirt     | stone  | stone  |  Stone overwrites dirt
	| grass    | stone  | stone  |    ... and grass
	| mycelium | stone  | stone  |    ... and mycelium
	| A        | stone  | A      |    ... but nothing else
	| A        | *      | A      |  Everything else is left as it is

	msSpongePrint:
	Used for most generators, it allows carving out air pockets, too, and uses the Sponge as the NOP block
	|    area block     |        |
	|   this   | Src    | result |
	+----------+--------+--------+
	| A        | sponge | A      |  Sponge is the NOP block
	| *        | B      | B      |  Everything else overwrites anything

	msDifference:
	Used to determine the differences between two areas. Only the differring blocks are preserved:
	|  area block  |        |
	| this | Src   | result |
	+------+-------+--------+
	| A    | A     | air    |  Same blocks are replaced with air
	| A    | non-A | A      |  Differring blocks are kept from "this"

	msSimpleCompare:
	Used to determine the differences between two areas. Blocks that differ are replaced with stone, same blocks are replaced with air
	|  area block  |        |
	| this | Src   | result |
	+------+-------+--------+
	| A    | A     | air    |  Same blocks are replaced with air
	| A    | non-A | stone  |  Differring blocks are replaced with stone

	msMask:
	Combines two areas, the blocks that are the same are kept, differing ones are reset to air
	|  area block  |        |
	| this | Src   | result |
	+------+-------+--------+
	| A    | A     | A      |  Same blocks are kept
	| A    | non-A | air    |  Everything else is replaced with air

	*/
	void Merge(const cBlockArea & a_Src, int a_RelX, int a_RelY, int a_RelZ, eMergeStrategy a_Strategy);

	/** Merges another block area into this one, using the specified block combinating strategy.
	See Merge() above for details. */
	void Merge(const cBlockArea & a_Src, const Vector3i & a_RelMinCoords, eMergeStrategy a_Strategy);

	/** Fills the entire block area with the specified data */
	void Fill(int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta = 0, NIBBLETYPE a_BlockLight = 0, NIBBLETYPE a_BlockSkyLight = 0x0f);

	// tolua_end

	/** Fills a cuboid inside the block area with the specified data */
	void FillRelCuboid(int a_MinRelX, int a_MaxRelX, int a_MinRelY, int a_MaxRelY, int a_MinRelZ, int a_MaxRelZ,
		int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta = 0,
		NIBBLETYPE a_BlockLight = 0, NIBBLETYPE a_BlockSkyLight = 0x0f
	);

	/** Fills a cuboid inside the block area with the specified data. a_Cuboid must be sorted. */
	void FillRelCuboid(const cCuboid & a_RelCuboid,
		int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta = 0,
		NIBBLETYPE a_BlockLight = 0, NIBBLETYPE a_BlockSkyLight = 0x0f
	);

	/** Draws a line between two points with the specified data. The line endpoints needn't be valid coords inside the area. */
	void RelLine(int a_RelX1, int a_RelY1, int a_RelZ1, int a_RelX2, int a_RelY2, int a_RelZ2,
		int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta = 0,
		NIBBLETYPE a_BlockLight = 0, NIBBLETYPE a_BlockSkyLight = 0x0f
	);

	/** Draws a line between two points with the specified data. The line endpoints needn't be valid coords inside the area. */
	void RelLine(const Vector3i & a_Point1, const Vector3i & a_Point2,
		int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta = 0,
		NIBBLETYPE a_BlockLight = 0, NIBBLETYPE a_BlockSkyLight = 0x0f
	);

	// tolua_begin

	/** Rotates the entire area counter-clockwise around the Y axis */
	void RotateCCW(void);

	/** Rotates the entire area clockwise around the Y axis */
	void RotateCW(void);

	/** Mirrors the entire area around the XY plane */
	void MirrorXY(void);

	/** Mirrors the entire area around the XZ plane */
	void MirrorXZ(void);

	/** Mirrors the entire area around the YZ plane */
	void MirrorYZ(void);

	/** Rotates the entire area counter-clockwise around the Y axis, doesn't use blockhandlers for block meta */
	void RotateCCWNoMeta(void);

	/** Rotates the entire area clockwise around the Y axis, doesn't use blockhandlers for block meta */
	void RotateCWNoMeta(void);

	/** Mirrors the entire area around the XY plane, doesn't use blockhandlers for block meta */
	void MirrorXYNoMeta(void);

	/** Mirrors the entire area around the XZ plane, doesn't use blockhandlers for block meta */
	void MirrorXZNoMeta(void);

	/** Mirrors the entire area around the YZ plane, doesn't use blockhandlers for block meta */
	void MirrorYZNoMeta(void);

	// tolua_end

	// Setters:
	void SetRelBlockType    (int a_RelX,   int a_RelY,   int a_RelZ,   BLOCKTYPE  a_BlockType);
	void SetBlockType       (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE  a_BlockType);
	void SetRelBlockMeta    (int a_RelX,   int a_RelY,   int a_RelZ,   NIBBLETYPE a_BlockMeta);
	void SetBlockMeta       (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockMeta);
	void SetRelBlockLight   (int a_RelX,   int a_RelY,   int a_RelZ,   NIBBLETYPE a_BlockLight);
	void SetBlockLight      (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockLight);
	void SetRelBlockSkyLight(int a_RelX,   int a_RelY,   int a_RelZ,   NIBBLETYPE a_BlockSkyLight);
	void SetBlockSkyLight   (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockSkyLight);

	// tolua_begin

	void SetWEOffset (int a_OffsetX, int a_OffsetY, int a_OffsetZ);
	void SetWEOffset (const Vector3i & a_Offset);
	const Vector3i & GetWEOffset  (void) const {return m_WEOffset;}

	// tolua_end

	// Getters:
	BLOCKTYPE  GetRelBlockType    (int a_RelX,   int a_RelY,   int a_RelZ)   const;
	BLOCKTYPE  GetBlockType       (int a_BlockX, int a_BlockY, int a_BlockZ) const;
	NIBBLETYPE GetRelBlockMeta    (int a_RelX,   int a_RelY,   int a_RelZ)   const;
	NIBBLETYPE GetBlockMeta       (int a_BlockX, int a_BlockY, int a_BlockZ) const;
	NIBBLETYPE GetRelBlockLight   (int a_RelX,   int a_RelY,   int a_RelZ)   const;
	NIBBLETYPE GetBlockLight      (int a_BlockX, int a_BlockY, int a_BlockZ) const;
	NIBBLETYPE GetRelBlockSkyLight(int a_RelX,   int a_RelY,   int a_RelZ)   const;
	NIBBLETYPE GetBlockSkyLight   (int a_BlockX, int a_BlockY, int a_BlockZ) const;

	void SetBlockTypeMeta   (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType,   NIBBLETYPE a_BlockMeta);
	void SetRelBlockTypeMeta(int a_RelX,   int a_RelY,   int a_RelZ,   BLOCKTYPE a_BlockType,   NIBBLETYPE a_BlockMeta);

	void GetBlockTypeMeta   (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const;
	void GetRelBlockTypeMeta(int a_RelX,   int a_RelY,   int a_RelZ,   BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const;

	const Vector3i & GetSize(void) const { return m_Size; }
	const Vector3i & GetOrigin(void) const { return m_Origin; }

	// tolua_begin

	cCuboid GetBounds(void) const;

	int GetSizeX(void) const { return m_Size.x; }
	int GetSizeY(void) const { return m_Size.y; }
	int GetSizeZ(void) const { return m_Size.z; }

	/** Returns the volume of the area, as number of blocks */
	int GetVolume(void) const { return m_Size.x * m_Size.y * m_Size.z; }

	int GetOriginX(void) const { return m_Origin.x; }
	int GetOriginY(void) const { return m_Origin.y; }
	int GetOriginZ(void) const { return m_Origin.z; }

	/** Returns the datatypes that are stored in the object (bitmask of baXXX values) */
	int GetDataTypes(void) const;

	bool HasBlockTypes    (void) const { return (m_BlockTypes    != nullptr); }
	bool HasBlockMetas    (void) const { return (m_BlockMetas    != nullptr); }
	bool HasBlockLights   (void) const { return (m_BlockLight    != nullptr); }
	bool HasBlockSkyLights(void) const { return (m_BlockSkyLight != nullptr); }
	bool HasBlockEntities (void) const { return (m_BlockEntities != nullptr); }

	/** Returns the count of blocks that are not air.
	Returns 0 if blocktypes not available. Block metas are ignored (if present, air with any meta is still considered air). */
	size_t CountNonAirBlocks(void) const;

	/** Returns how many times the specified block is contained in the area.
	The blocks' meta values are ignored, only the blocktype is compared. */
	size_t CountSpecificBlocks(BLOCKTYPE a_BlockType) const;

	/** Returns how many times the specified block is contained in the area.
	Both the block's type and meta must match in order to be counted in.
	If the block metas aren't present in the area, logs a warning and ignores the meta specification. */
	size_t CountSpecificBlocks(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) const;

	// tolua_end

	/** Returns the minimum and maximum coords in each direction for the first non-ignored block in each direction.
	If there are no non-ignored blocks within the area, or blocktypes are not present, the returned values are reverse-ranges (MinX <- m_RangeX, MaxX <- 0 etc.)
	Exported to Lua in ManualBindings.cpp. */
	void GetNonAirCropRelCoords(int & a_MinRelX, int & a_MinRelY, int & a_MinRelZ, int & a_MaxRelX, int & a_MaxRelY, int & a_MaxRelZ, BLOCKTYPE a_IgnoreBlockType = E_BLOCK_AIR);

	// Clients can use these for faster access to all blocktypes. Be careful though!
	/** Returns the internal pointer to the block types */
	BLOCKTYPE *  GetBlockTypes   (void) const { return m_BlockTypes.get();    }
	NIBBLETYPE * GetBlockMetas   (void) const { return m_BlockMetas.get();    }  // NOTE: one byte per block!
	NIBBLETYPE * GetBlockLight   (void) const { return m_BlockLight.get();    }  // NOTE: one byte per block!
	NIBBLETYPE * GetBlockSkyLight(void) const { return m_BlockSkyLight.get(); }  // NOTE: one byte per block!
	size_t       GetBlockCount(void) const { return static_cast<size_t>(m_Size.x * m_Size.y * m_Size.z); }
	static size_t MakeIndexForSize(Vector3i a_RelPos, Vector3i a_Size);

	/** Returns the index into the internal arrays for the specified coords */
	size_t MakeIndex(Vector3i a_RelPos) const
	{
		return MakeIndexForSize(a_RelPos, m_Size);
	}

	/** OBSOLETE, use the Vector3i-based overload instead.
	Returns the index into the internal arrays for the specified coords */
	size_t MakeIndex(int a_RelX, int a_RelY, int a_RelZ) const
	{
		return MakeIndexForSize({ a_RelX, a_RelY, a_RelZ }, m_Size);
	}

	/** Calls the callback for the block entity at the specified coords.
	Returns false if there is no block entity at those coords, or the block area doesn't have baBlockEntities.
	Returns the value that the callback has returned if there is a block entity. */
	bool DoWithBlockEntityRelAt(int a_RelX, int a_RelY, int a_RelZ, cBlockEntityCallback a_Callback);

	/** Calls the callback for the block entity at the specified coords.
	Returns false if there is no block entity at those coords.
	Returns the value that the callback has returned if there is a block entity. */
	bool DoWithBlockEntityAt   (int a_BlockX, int a_BlockY, int a_BlockZ, cBlockEntityCallback a_Callback);

	/** Calls the callback for all the block entities.
	If the callback returns true, aborts the enumeration and returns false.
	If the callback returns true, continues with the next BE.
	Returns true if all block entities have been enumerated (including the case when there is none or the area is without baBlockEntities). */
	bool ForEachBlockEntity(cBlockEntityCallback a_Callback);

	/** Direct read-only access to block entities. */
	const cBlockEntities & GetBlockEntities(void) const { ASSERT(HasBlockEntities()); return *m_BlockEntities; }



protected:

	friend class cChunkDesc;
	friend class cSchematicFileSerializer;

	class cChunkReader:
		public cChunkDataCallback
	{
	public:
		cChunkReader(cBlockArea & a_Area);

	protected:
		cBlockArea & m_Area;
		cCuboid m_AreaBounds;  ///< Bounds of the whole area being read, in world coords
		Vector3i m_Origin;
		int m_CurrentChunkX;
		int m_CurrentChunkZ;

		void CopyNibbles(NIBBLETYPE * a_AreaDst, const NIBBLETYPE * a_ChunkSrc);

		// cChunkDataCallback overrides:
		virtual bool Coords(int a_ChunkX, int a_ChunkZ) override;
		virtual void ChunkData(const cChunkData &  a_BlockTypes) override;
		virtual void BlockEntity(cBlockEntity * a_BlockEntity) override;
	} ;

	using NIBBLEARRAY = std::unique_ptr<NIBBLETYPE[]>;
	using BLOCKARRAY = std::unique_ptr<BLOCKTYPE[]>;
	using cBlockEntitiesPtr = std::unique_ptr<cBlockEntities>;

	Vector3i m_Origin;
	Vector3i m_Size;

	/** An extra data value sometimes stored in the .schematic file. Used mainly by the WorldEdit plugin.
	cBlockArea doesn't use this value in any way. */
	Vector3i m_WEOffset;

	BLOCKARRAY  m_BlockTypes;
	NIBBLEARRAY m_BlockMetas;     // Each meta is stored as a separate byte for faster access
	NIBBLEARRAY m_BlockLight;     // Each light value is stored as a separate byte for faster access
	NIBBLEARRAY m_BlockSkyLight;  // Each light value is stored as a separate byte for faster access

	/** The block entities contained within the area.
	Only valid if the area was created / read with the baBlockEntities flag.
	The block entities are owned by this object. */
	cBlockEntitiesPtr m_BlockEntities;

	/** Clears the data stored and prepares a fresh new block area with the specified dimensions */
	bool SetSize(int a_SizeX, int a_SizeY, int a_SizeZ, int a_DataTypes);

	// Basic Setters:
	void SetRelNibble(int a_RelX,   int a_RelY,   int a_RelZ,   NIBBLETYPE a_Value, NIBBLETYPE * a_Array);
	void SetNibble   (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Value, NIBBLETYPE * a_Array);

	// Basic Getters:
	NIBBLETYPE GetRelNibble(int a_RelX,   int a_RelY,   int a_RelZ,   NIBBLETYPE * a_Array) const;
	NIBBLETYPE GetNibble   (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE * a_Array) const;

	// Crop helpers:
	void CropBlockTypes(int a_AddMinX, int a_SubMaxX, int a_AddMinY, int a_SubMaxY, int a_AddMinZ, int a_SubMaxZ);
	void CropNibbles   (NIBBLEARRAY & a_Array, int a_AddMinX, int a_SubMaxX, int a_AddMinY, int a_SubMaxY, int a_AddMinZ, int a_SubMaxZ);

	// Expand helpers:
	void ExpandBlockTypes(int a_SubMinX, int a_AddMaxX, int a_SubMinY, int a_AddMaxY, int a_SubMinZ, int a_AddMaxZ);
	void ExpandNibbles   (NIBBLEARRAY & a_Array, int a_SubMinX, int a_AddMaxX, int a_SubMinY, int a_AddMaxY, int a_SubMinZ, int a_AddMaxZ);

	/** Sets the specified datatypes at the specified location.
	If the coords are not valid, ignores the call (so that RelLine() can work simply). */
	void RelSetData(
		int a_RelX, int a_RelY, int a_RelZ,
		int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta,
		NIBBLETYPE a_BlockLight, NIBBLETYPE a_BlockSkyLight
	);

	template <bool MetasValid>
	void MergeByStrategy(const cBlockArea & a_Src, int a_RelX, int a_RelY, int a_RelZ, eMergeStrategy a_Strategy, const NIBBLETYPE * SrcMetas, NIBBLETYPE * DstMetas);

	/** Updates m_BlockEntities to remove BEs that no longer match the blocktype at their coords, and clones from a_Src the BEs that are missing.
	a_RelX, a_RelY and a_RelZ are relative coords that should be added to all BEs from a_Src before checking them.
	If a block should have a BE but one cannot be found in either this or a_Src, a new one is created. */
	void MergeBlockEntities(int a_RelX, int a_RelY, int a_RelZ, const cBlockArea & a_Src);

	/** Updates m_BlockEntities to remove BEs that no longer match the blocktype at their coords, and add new BEs that are missing. */
	void RescanBlockEntities(void);

	/** Removes from m_BlockEntities those BEs that no longer match the blocktype at their coords. */
	void RemoveNonMatchingBlockEntities(void);

	/** Returns the cBlockEntity at the specified coords, or nullptr if none. */
	cBlockEntity * GetBlockEntityRel(Vector3i a_RelPos);

	// tolua_begin
} ;
// tolua_end




