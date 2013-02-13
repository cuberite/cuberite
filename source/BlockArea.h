
// BlockArea.h

// Interfaces to the cBlockArea object representing an area of block data that can be queried from cWorld and then accessed again without further queries
// The object also supports writing the blockdata back into cWorld, even into other coords

// NOTE: All Nibble values (meta, blocklight, skylight) are stored one-nibble-per-byte for faster access / editting!





#pragma once





// fwd: World.h
class cWorld;

// fwd: FastNBT.h
class cParsedNBT;





// tolua_begin
class cBlockArea
{
	// tolua_end
	DISALLOW_COPY_AND_ASSIGN(cBlockArea);
	// tolua_begin
	
public:

	/// What data is to be queried (bit-mask)
	enum
	{
		baTypes    = 1,
		baMetas    = 2,
		baLight    = 4,
		baSkyLight = 8,
	} ;
	
	enum eMergeStrategy
	{
		msOverwrite,
		msFillAir,
		msImprint,
	} ;
	
	cBlockArea(void);
	~cBlockArea();
	
	/// Clears the data stored to reclaim memory
	void Clear(void);
	
	/** Creates a new area of the specified size and contents. 
	Origin is set to all zeroes. 
	BlockTypes are set to air, block metas to zero, blocklights to zero and skylights to full light.
	*/
	void Create(int a_SizeX, int a_SizeY, int a_SizeZ, int a_DataTypes = baTypes | baMetas);
	
	/// Resets the origin. No other changes are made, contents are untouched.
	void SetOrigin(int a_OriginX, int a_OriginY, int a_OriginZ);
	
	/// Reads an area of blocks specified. Returns true if successful. All coords are inclusive.
	bool Read(cWorld * a_World, int a_MinBlockX, int a_MaxBlockX, int a_MinBlockY, int a_MaxBlockY, int a_MinBlockZ, int a_MaxBlockZ, int a_DataTypes = baTypes | baMetas);
	
	// TODO: Write() is not too good an interface: if it fails, there's no way to repeat only for the parts that didn't write
	// A better way may be to return a list of cBlockAreas for each part that didn't succeed writing, so that the caller may try again
	
	/// Writes the area back into cWorld at the coords specified. Returns true if successful in all chunks, false if only partially / not at all
	bool Write(cWorld * a_World, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes = baTypes | baMetas);
	
	/// Copies this object's contents into the specified BlockArea.
	void CopyTo(cBlockArea & a_Into) const;
	
	/// Copies the contents from the specified BlockArea into this object.
	void CopyFrom(const cBlockArea & a_From);
	
	/// For testing purposes only, dumps the area into a file.
	void DumpToRawFile(const AString & a_FileName);
	
	/// Loads an area from a .schematic file. Returns true if successful
	bool LoadFromSchematicFile(const AString & a_FileName);
	
	/// Saves the area into a .schematic file. Returns true if successful
	bool SaveToSchematicFile(const AString & a_FileName);
	
	/// Crops the internal contents by the specified amount of blocks from each border.
	void Crop(int a_AddMinX, int a_SubMaxX, int a_AddMinY, int a_SubMaxY, int a_AddMinZ, int a_SubMaxZ);
	
	/// Expands the internal contents by the specified amount of blocks from each border
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
	*/
	void Merge(const cBlockArea & a_Src, int a_RelX, int a_RelY, int a_RelZ, eMergeStrategy a_Strategy);
	
	/// Fills the entire block area with the specified data
	void Fill(int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta = 0, NIBBLETYPE a_BlockLight = 0, NIBBLETYPE a_BlockSkyLight = 0x0f);
	
	/// Fills a cuboid inside the block area with the specified data
	void FillRelCuboid(int a_MinRelX, int a_MaxRelX, int a_MinRelY, int a_MaxRelY, int a_MinRelZ, int a_MaxRelZ, 
		int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta = 0,
		NIBBLETYPE a_BlockLight = 0, NIBBLETYPE a_BlockSkyLight = 0x0f
	);
	
	/// Draws a line from between two points with the specified data
	void RelLine(int a_RelX1, int a_RelY1, int a_RelZ1, int a_RelX2, int a_RelY2, int a_RelZ2,
		int a_DataTypes, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta = 0,
		NIBBLETYPE a_BlockLight = 0, NIBBLETYPE a_BlockSkyLight = 0x0f
	);
	
	// Setters:
	void SetRelBlockType    (int a_RelX,   int a_RelY,   int a_RelZ,   BLOCKTYPE  a_BlockType);
	void SetBlockType       (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE  a_BlockType);
	void SetRelBlockMeta    (int a_RelX,   int a_RelY,   int a_RelZ,   NIBBLETYPE a_BlockMeta);
	void SetBlockMeta       (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockMeta);
	void SetRelBlockLight   (int a_RelX,   int a_RelY,   int a_RelZ,   NIBBLETYPE a_BlockLight);
	void SetBlockLight      (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockLight);
	void SetRelBlockSkyLight(int a_RelX,   int a_RelY,   int a_RelZ,   NIBBLETYPE a_BlockSkyLight);
	void SetBlockSkyLight   (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_BlockSkyLight);

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
	
	int GetSizeX(void) const { return m_SizeX; }
	int GetSizeY(void) const { return m_SizeY; }
	int GetSizeZ(void) const { return m_SizeZ; }
	
	int GetOriginX(void) const { return m_OriginX; }
	int GetOriginY(void) const { return m_OriginY; }
	int GetOriginZ(void) const { return m_OriginZ; }
	
	/// Returns the datatypes that are stored in the object (bitmask of baXXX values)
	int GetDataTypes(void) const;
	
	bool HasBlockTypes    (void) const { return (m_BlockTypes    != NULL); }
	bool HasBlockMetas    (void) const { return (m_BlockMetas    != NULL); }
	bool HasBlockLights   (void) const { return (m_BlockLight    != NULL); }
	bool HasBlockSkyLights(void) const { return (m_BlockSkyLight != NULL); }
	
	// tolua_end
	
	// Clients can use these for faster access to all blocktypes. Be careful though!
	/// Returns the internal pointer to the block types
	BLOCKTYPE *  GetBlockTypes   (void) const { return m_BlockTypes; }
	NIBBLETYPE * GetBlockMetas   (void) const { return m_BlockMetas; }     // NOTE: one byte per block!
	NIBBLETYPE * GetBlockLight   (void) const { return m_BlockLight; }     // NOTE: one byte per block!
	NIBBLETYPE * GetBlockSkyLight(void) const { return m_BlockSkyLight; }  // NOTE: one byte per block!
	int          GetBlockCount(void) const { return m_SizeX * m_SizeY * m_SizeZ; }
	int MakeIndex(int a_RelX, int a_RelY, int a_RelZ) const;

protected:
	friend class cChunkDesc;
	
	class cChunkReader :
		public cChunkDataCallback
	{
	public:
		cChunkReader(cBlockArea & a_Area);
		
	protected:
		cBlockArea & m_Area;
		int m_OriginX;
		int m_OriginY;
		int m_OriginZ;
		int m_CurrentChunkX;
		int m_CurrentChunkZ;
		
		void CopyNibbles(NIBBLETYPE * a_AreaDst, const NIBBLETYPE * a_ChunkSrc);
		
		// cChunkDataCallback overrides:
		virtual bool Coords       (int a_ChunkX, int a_ChunkZ) override;
		virtual void BlockTypes   (const BLOCKTYPE *  a_BlockTypes)    override;
		virtual void BlockMeta    (const NIBBLETYPE * a_BlockMetas)    override;
		virtual void BlockLight   (const NIBBLETYPE * a_BlockLight)    override;
		virtual void BlockSkyLight(const NIBBLETYPE * a_BlockSkyLight) override;
	} ;
	
	typedef NIBBLETYPE * NIBBLEARRAY;
	
	
	int m_OriginX;
	int m_OriginY;
	int m_OriginZ;
	int m_SizeX;
	int m_SizeY;
	int m_SizeZ;
	
	BLOCKTYPE *  m_BlockTypes;
	NIBBLETYPE * m_BlockMetas;     // Each meta is stored as a separate byte for faster access
	NIBBLETYPE * m_BlockLight;     // Each light value is stored as a separate byte for faster access
	NIBBLETYPE * m_BlockSkyLight;  // Each light value is stored as a separate byte for faster access
	
	/// Clears the data stored and prepares a fresh new block area with the specified dimensions
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
	
	/// Loads the area from a schematic file uncompressed and parsed into a NBT tree. Returns true if successful.
	bool LoadFromSchematicNBT(cParsedNBT & a_NBT);

	// tolua_begin
} ;
// tolua_end




