
// BlockArea.h

// Interfaces to the cBlockArea object representing an area of block data that can be queried from cWorld and then accessed again without further queries
// The object also supports writing the blockdata back into cWorld, even into other coords

// NOTE: All Nibble values (meta, blocklight, skylight) are stored one-nibble-per-byte for faster access / editting!





#pragma once

#include "ForEachChunkProvider.h"
#include "Vector3.h"
#include "ChunkDataCallback.h"




// fwd:
class cCuboid;





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
		baTypes    = 1,
		baMetas    = 2,
		baLight    = 4,
		baSkyLight = 8,
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
		msMask,
	} ;
	
	cBlockArea(void);
	~cBlockArea();
	
	/** Clears the data stored to reclaim memory */
	void Clear(void);
	
	/** Creates a new area of the specified size and contents.
	Origin is set to all zeroes.
	BlockTypes are set to air, block metas to zero, blocklights to zero and skylights to full light.
	*/
	void Create(int a_SizeX, int a_SizeY, int a_SizeZ, int a_DataTypes = baTypes | baMetas);
	
	/** Creates a new area of the specified size and contents.
	Origin is set to all zeroes.
	BlockTypes are set to air, block metas to zero, blocklights to zero and skylights to full light.
	*/
	void Create(const Vector3i & a_Size, int a_DataTypes = baTypes | baMetas);
	
	/** Resets the origin. No other changes are made, contents are untouched. */
	void SetOrigin(int a_OriginX, int a_OriginY, int a_OriginZ);
	
	/** Resets the origin. No other changes are made, contents are untouched. */
	void SetOrigin(const Vector3i & a_Origin);
	
	/** Reads an area of blocks specified. Returns true if successful. All coords are inclusive. */
	bool Read(cForEachChunkProvider * a_ForEachChunkProvider, int a_MinBlockX, int a_MaxBlockX, int a_MinBlockY, int a_MaxBlockY, int a_MinBlockZ, int a_MaxBlockZ, int a_DataTypes = baTypes | baMetas);
	
	/** Reads an area of blocks specified. Returns true if successful. The bounds are included in the read area. */
	bool Read(cForEachChunkProvider * a_ForEachChunkProvider, const cCuboid & a_Bounds, int a_DataTypes = baTypes | baMetas);
	
	/** Reads an area of blocks specified. Returns true if successful. The bounds are included in the read area. */
	bool Read(cForEachChunkProvider * a_ForEachChunkProvider, const Vector3i & a_Point1, const Vector3i & a_Point2, int a_DataTypes = baTypes | baMetas);
	
	// TODO: Write() is not too good an interface: if it fails, there's no way to repeat only for the parts that didn't write
	// A better way may be to return a list of cBlockAreas for each part that didn't succeed writing, so that the caller may try again
	
	/** Writes the area back into cWorld at the coords specified. Returns true if successful in all chunks, false if only partially / not at all */
	bool Write(cForEachChunkProvider * a_ForEachChunkProvider, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes = baTypes | baMetas);
	
	/** Writes the area back into cWorld at the coords specified. Returns true if successful in all chunks, false if only partially / not at all */
	bool Write(cForEachChunkProvider * a_ForEachChunkProvider, const Vector3i & a_MinCoords, int a_DataTypes = baTypes | baMetas);
	
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
	
	/** Draws a line from between two points with the specified data */
	void RelLine(int a_RelX1, int a_RelY1, int a_RelZ1, int a_RelX2, int a_RelY2, int a_RelZ2,
		int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta = 0,
		NIBBLETYPE a_BlockLight = 0, NIBBLETYPE a_BlockSkyLight = 0x0f
	);
	
	/** Draws a line from between two points with the specified data */
	void RelLine(const Vector3i & a_Point1, const Vector3i & a_Point2,
		int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta = 0,
		NIBBLETYPE a_BlockLight = 0, NIBBLETYPE a_BlockSkyLight = 0x0f
	);
	
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
	
	// Setters:
	void SetRelBlockType    (int a_RelX,   int a_RelY,   int a_RelZ,   BLOCKTYPE  a_BlockType);
	void SetBlockType       (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE  a_BlockType);
	void SetRelBlockMeta    (int a_RelX,   int a_RelY,   int a_RelZ,   NIBBLETYPE a_BlockMeta);
	void SetBlockMeta       (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockMeta);
	void SetRelBlockLight   (int a_RelX,   int a_RelY,   int a_RelZ,   NIBBLETYPE a_BlockLight);
	void SetBlockLight      (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockLight);
	void SetRelBlockSkyLight(int a_RelX,   int a_RelY,   int a_RelZ,   NIBBLETYPE a_BlockSkyLight);
	void SetBlockSkyLight   (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockSkyLight);
	void SetWEOffset        (int a_OffsetX, int a_OffsetY, int a_OffsetZ);
	void SetWEOffset        (const Vector3i & a_Offset);

	// Getters:
	BLOCKTYPE  GetRelBlockType    (int a_RelX,   int a_RelY,   int a_RelZ)   const;
	BLOCKTYPE  GetBlockType       (int a_BlockX, int a_BlockY, int a_BlockZ) const;
	NIBBLETYPE GetRelBlockMeta    (int a_RelX,   int a_RelY,   int a_RelZ)   const;
	NIBBLETYPE GetBlockMeta       (int a_BlockX, int a_BlockY, int a_BlockZ) const;
	NIBBLETYPE GetRelBlockLight   (int a_RelX,   int a_RelY,   int a_RelZ)   const;
	NIBBLETYPE GetBlockLight      (int a_BlockX, int a_BlockY, int a_BlockZ) const;
	NIBBLETYPE GetRelBlockSkyLight(int a_RelX,   int a_RelY,   int a_RelZ)   const;
	NIBBLETYPE GetBlockSkyLight   (int a_BlockX, int a_BlockY, int a_BlockZ) const;
	const Vector3i & GetWEOffset  (void)                                     const {return m_WEOffset;}

	void SetBlockTypeMeta   (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType,   NIBBLETYPE a_BlockMeta);
	void SetRelBlockTypeMeta(int a_RelX,   int a_RelY,   int a_RelZ,   BLOCKTYPE a_BlockType,   NIBBLETYPE a_BlockMeta);
	
	// tolua_end
	
	// These need manual exporting, tolua generates the binding as requiring 2 extra input params
	void GetBlockTypeMeta   (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const;
	void GetRelBlockTypeMeta(int a_RelX,   int a_RelY,   int a_RelZ,   BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const;
	
	// GetSize() is already exported manually to return 3 numbers, can't auto-export
	const Vector3i & GetSize(void) const { return m_Size; }
	
	// GetOrigin() is already exported manually to return 3 numbers, can't auto-export
	const Vector3i & GetOrigin(void) const { return m_Origin; }
	
	// tolua_begin
	
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
	
	bool HasBlockTypes    (void) const { return (m_BlockTypes    != NULL); }
	bool HasBlockMetas    (void) const { return (m_BlockMetas    != NULL); }
	bool HasBlockLights   (void) const { return (m_BlockLight    != NULL); }
	bool HasBlockSkyLights(void) const { return (m_BlockSkyLight != NULL); }
	
	// tolua_end
	
	// Clients can use these for faster access to all blocktypes. Be careful though!
	/** Returns the internal pointer to the block types */
	BLOCKTYPE *  GetBlockTypes   (void) const { return m_BlockTypes; }
	NIBBLETYPE * GetBlockMetas   (void) const { return m_BlockMetas; }     // NOTE: one byte per block!
	NIBBLETYPE * GetBlockLight   (void) const { return m_BlockLight; }     // NOTE: one byte per block!
	NIBBLETYPE * GetBlockSkyLight(void) const { return m_BlockSkyLight; }  // NOTE: one byte per block!
	size_t       GetBlockCount(void) const { return (size_t)(m_Size.x * m_Size.y * m_Size.z); }
	int MakeIndex(int a_RelX, int a_RelY, int a_RelZ) const;

protected:
	friend class cChunkDesc;
	friend class cSchematicFileSerializer;
	
	class cChunkReader :
		public cChunkDataCallback
	{
	public:
		cChunkReader(cBlockArea & a_Area);
		
	protected:
		cBlockArea & m_Area;
		Vector3i m_Origin;
		int m_CurrentChunkX;
		int m_CurrentChunkZ;
		
		void CopyNibbles(NIBBLETYPE * a_AreaDst, const NIBBLETYPE * a_ChunkSrc);
		
		// cChunkDataCallback overrides:
		virtual bool Coords(int a_ChunkX, int a_ChunkZ) override;
		virtual void ChunkData(const cChunkData &  a_BlockTypes) override;
	} ;
	
	typedef NIBBLETYPE * NIBBLEARRAY;
	
	
	Vector3i m_Origin;
	Vector3i m_Size;
	
	/** An extra data value sometimes stored in the .schematic file. Used mainly by the WorldEdit plugin.
	cBlockArea doesn't use this value in any way. */
	Vector3i m_WEOffset;

	BLOCKTYPE *  m_BlockTypes;
	NIBBLETYPE * m_BlockMetas;     // Each meta is stored as a separate byte for faster access
	NIBBLETYPE * m_BlockLight;     // Each light value is stored as a separate byte for faster access
	NIBBLETYPE * m_BlockSkyLight;  // Each light value is stored as a separate byte for faster access
	
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

	/** Sets the specified datatypes at the specified location. */
	void RelSetData(
		int a_RelX, int a_RelY, int a_RelZ,
		int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta,
		NIBBLETYPE a_BlockLight, NIBBLETYPE a_BlockSkyLight
	);
	
	template<bool MetasValid>
	void MergeByStrategy(const cBlockArea & a_Src, int a_RelX, int a_RelY, int a_RelZ, eMergeStrategy a_Strategy, const NIBBLETYPE * SrcMetas, NIBBLETYPE * DstMetas);
	// tolua_begin
} ;
// tolua_end




