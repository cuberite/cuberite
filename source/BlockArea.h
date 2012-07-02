
// BlockArea.h

// Interfaces to the cBlockArea object representing an area of block data that can be queried from cWorld and then accessed again without further queries
// The object also supports writing the blockdata back into cWorld, even into other coords





#pragma once





// fwd: "cWorld.h"
class cWorld;





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
	
	cBlockArea(void);
	~cBlockArea();
	
	/// Clears the data stored to reclaim memory
	void Clear(void);
	
	/// Reads an area of blocks specified. Returns true if successful. All coords are inclusive.
	bool Read(cWorld * a_World, int a_MinBlockX, int a_MaxBlockX, int a_MinBlockY, int a_MaxBlockY, int a_MinBlockZ, int a_MaxBlockZ, int a_DataTypes = baTypes | baMetas);
	
	/// Writes the area back into cWorld at the coords specified. Returns true if successful.
	bool Write(cWorld * a_World, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes = baTypes | baMetas);
	
	// TODO: Write() is not too good an interface: if it fails, there's no way to repeat only for the parts that didn't write
	// A better way may be to return a list of cBlockAreas for each part that didn't succeed writing, so that the caller may try again
	
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
	BLOCKTYPE  GetRelBlockType    (int a_RelX,   int a_RelY,   int a_RelZ);
	BLOCKTYPE  GetBlockType       (int a_BlockX, int a_BlockY, int a_BlockZ);
	NIBBLETYPE GetRelBlockMeta    (int a_RelX,   int a_RelY,   int a_RelZ);
	NIBBLETYPE GetBlockMeta       (int a_BlockX, int a_BlockY, int a_BlockZ);
	NIBBLETYPE GetRelBlockLight   (int a_RelX,   int a_RelY,   int a_RelZ);
	NIBBLETYPE GetBlockLight      (int a_BlockX, int a_BlockY, int a_BlockZ);
	NIBBLETYPE GetRelBlockSkyLight(int a_RelX,   int a_RelY,   int a_RelZ);
	NIBBLETYPE GetBlockSkyLight   (int a_BlockX, int a_BlockY, int a_BlockZ);
	
	/// Returns the datatypes that are stored in the object (bitmask of baXXX values)
	int GetDataTypes(void) const;
	
	// tolua_end
	
	// Clients can use these for faster access to all blocktypes. Be careful though!
	/// Returns the internal pointer to the block types
	BLOCKTYPE * GetBlockTypes(void) { return m_BlockTypes; }
	int         GetBlockCount(void) const { return m_SizeX * m_SizeY * m_SizeZ; }
	
	// tolua_begin

protected:

	// tolua_end
	
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
	
	// tolua_begin
	
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
	
	bool SetSize(int a_SizeX, int a_SizeY, int a_SizeZ, int a_DataTypes);
	
	int MakeIndex(int a_RelX, int a_RelY, int a_RelZ);
	
	// Basic Setters:
	void SetRelNibble(int a_RelX,   int a_RelY,   int a_RelZ,   NIBBLETYPE a_Value, NIBBLETYPE * a_Array);
	void SetNibble   (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Value, NIBBLETYPE * a_Array);

	// Basic Getters:
	NIBBLETYPE GetRelNibble(int a_RelX,   int a_RelY,   int a_RelZ,   NIBBLETYPE * a_Array);
	NIBBLETYPE GetNibble   (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE * a_Array);
} ;
// tolua_end




