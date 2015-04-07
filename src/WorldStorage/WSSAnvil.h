
// WSSAnvil.h

// Interfaces to the cWSSAnvil class representing the Anvil world storage scheme




#pragma once

#include "WorldStorage.h"
#include "FastNBT.h"
#include "../Mobs/Monster.h"





// fwd: ItemGrid.h
class cItemGrid;

class cProjectileEntity;
class cHangingEntity;
class cWolf;





enum
{
	/// Maximum number of chunks in an MCA file - also the count of the header items
	MCA_MAX_CHUNKS = 32 * 32,
	
	/// The MCA header is 8 KiB
	MCA_HEADER_SIZE = MCA_MAX_CHUNKS * 8,
	
	/// There are 5 bytes of header in front of each chunk
	MCA_CHUNK_HEADER_LENGTH = 5,
} ;





class cWSSAnvil :
	public cWSSchema
{
	typedef cWSSchema super;
	
public:

	cWSSAnvil(cWorld * a_World, int a_CompressionFactor);
	virtual ~cWSSAnvil();
	
protected:

	class cMCAFile
	{
	public:
	
		cMCAFile(const AString & a_FileName, int a_RegionX, int a_RegionZ);
		
		bool GetChunkData  (const cChunkCoords & a_Chunk, AString & a_Data);
		bool SetChunkData  (const cChunkCoords & a_Chunk, const AString & a_Data);
		bool EraseChunkData(const cChunkCoords & a_Chunk);
		
		int             GetRegionX (void) const {return m_RegionX; }
		int             GetRegionZ (void) const {return m_RegionZ; }
		const AString & GetFileName(void) const {return m_FileName; }
		
	protected:
	
		int     m_RegionX;
		int     m_RegionZ;
		cFile   m_File;
		AString m_FileName;
		
		// The header, copied from the file so we don't have to seek to it all the time
		// First 1024 entries are chunk locations - the 3 + 1 byte sector-offset and sector-count
		unsigned m_Header[MCA_MAX_CHUNKS];
		
		// Chunk timestamps, following the chunk headers
		unsigned m_TimeStamps[MCA_MAX_CHUNKS];
		
		/// Finds a free location large enough to hold a_Data. Gets a hint of the chunk coords, places the data there if it fits. Returns the sector number.
		unsigned FindFreeLocation(int a_LocalX, int a_LocalZ, const AString & a_Data);
		
		/// Opens a MCA file either for a Read operation (fails if doesn't exist) or for a Write operation (creates new if not found)
		bool OpenFile(bool a_IsForReading);
	} ;
	typedef std::list<cMCAFile *> cMCAFiles;
	
	cCriticalSection m_CS;
	cMCAFiles        m_Files;  // a MRU cache of MCA files
	
	int m_CompressionFactor;

	/// Gets chunk data from the correct file; locks file CS as needed
	bool GetChunkData(const cChunkCoords & a_Chunk, AString & a_Data);

	/// Sets chunk data into the correct file; locks file CS as needed
	bool SetChunkData(const cChunkCoords & a_Chunk, const AString & a_Data);

	/// Loads the chunk from the data (no locking needed)
	bool LoadChunkFromData(const cChunkCoords & a_Chunk, const AString & a_Data);
	
	/// Saves the chunk into datastream (no locking needed)
	bool SaveChunkToData(const cChunkCoords & a_Chunk, AString & a_Data);
	
	/// Loads the chunk from NBT data (no locking needed)
	bool LoadChunkFromNBT(const cChunkCoords & a_Chunk, const cParsedNBT & a_NBT);
	
	/// Saves the chunk into NBT data using a_Writer; returns true on success
	bool SaveChunkToNBT(const cChunkCoords & a_Chunk, cFastNBTWriter & a_Writer);
	
	/// Loads the chunk's biome map from vanilla-format; returns a_BiomeMap if biomes present and valid, nullptr otherwise
	cChunkDef::BiomeMap * LoadVanillaBiomeMapFromNBT(cChunkDef::BiomeMap * a_BiomeMap, const cParsedNBT & a_NBT, int a_TagIdx);
	
	/// Loads the chunk's biome map from MCS format; returns a_BiomeMap if biomes present and valid, nullptr otherwise
	cChunkDef::BiomeMap * LoadBiomeMapFromNBT(cChunkDef::BiomeMap * a_BiomeMap, const cParsedNBT & a_NBT, int a_TagIdx);
	
	/// Loads the chunk's entities from NBT data (a_Tag is the Level\\Entities list tag; may be -1)
	void LoadEntitiesFromNBT(cEntityList & a_Entitites, const cParsedNBT & a_NBT, int a_Tag);
	
	/// Loads the chunk's BlockEntities from NBT data (a_Tag is the Level\\TileEntities list tag; may be -1)
	void LoadBlockEntitiesFromNBT(cBlockEntityList & a_BlockEntitites, const cParsedNBT & a_NBT, int a_Tag, BLOCKTYPE * a_BlockTypes, NIBBLETYPE * a_BlockMetas);
	
	/** Loads the data for a block entity from the specified NBT tag.
	Returns the loaded block entity, or nullptr upon failure. */
	cBlockEntity * LoadBlockEntityFromNBT(const cParsedNBT & a_NBT, int a_Tag, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	/// Loads a cItem contents from the specified NBT tag; returns true if successful. Doesn't load the Slot tag
	bool LoadItemFromNBT(cItem & a_Item, const cParsedNBT & a_NBT, int a_TagIdx);
	
	/** Loads contentents of an Items[] list tag into a cItemGrid
	ItemGrid begins at the specified slot offset
	Slots outside the ItemGrid range are ignored
	*/
	void LoadItemGridFromNBT(cItemGrid & a_ItemGrid, const cParsedNBT & a_NBT, int a_ItemsTagIdx, int s_SlotOffset = 0);
	
	/** Returns true iff the "id" child tag inside the specified tag equals the specified expected type. */
	bool CheckBlockEntityType(const cParsedNBT & a_NBT, int a_TagIdx, const char * a_ExpectedType);

	cBlockEntity * LoadBeaconFromNBT      (const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ);
	cBlockEntity * LoadChestFromNBT       (const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_ChestBlockType);
	cBlockEntity * LoadCommandBlockFromNBT(const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ);
	cBlockEntity * LoadDispenserFromNBT   (const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ);
	cBlockEntity * LoadDropperFromNBT     (const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ);
	cBlockEntity * LoadFlowerPotFromNBT   (const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ);
	cBlockEntity * LoadFurnaceFromNBT     (const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	cBlockEntity * LoadMobSpawnerFromNBT  (const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ);
	cBlockEntity * LoadHopperFromNBT      (const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ);
	cBlockEntity * LoadJukeboxFromNBT     (const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ);
	cBlockEntity * LoadMobHeadFromNBT     (const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ);
	cBlockEntity * LoadNoteBlockFromNBT   (const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ);
	cBlockEntity * LoadSignFromNBT        (const cParsedNBT & a_NBT, int a_TagIdx, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_SignBlockType);
	
	void LoadEntityFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_EntityTagIdx, const char * a_IDTag, size_t a_IDTagLength);
	
	void LoadBoatFromNBT            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadEnderCrystalFromNBT    (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadFallingBlockFromNBT    (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadPickupFromNBT          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadTNTFromNBT             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadExpOrbFromNBT          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadHangingFromNBT         (cHangingEntity & a_Hanging, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadItemFrameFromNBT       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadPaintingFromNBT        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);

	void LoadMinecartRFromNBT       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadMinecartCFromNBT       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadMinecartFFromNBT       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadMinecartTFromNBT       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadMinecartHFromNBT       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);

	void LoadArrowFromNBT           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadSplashPotionFromNBT    (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadSnowballFromNBT        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadEggFromNBT             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadFireballFromNBT        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadFireChargeFromNBT      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadThrownEnderpearlFromNBT(cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);

	void LoadBatFromNBT             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadBlazeFromNBT           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadCaveSpiderFromNBT      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadChickenFromNBT         (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadCowFromNBT             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadCreeperFromNBT         (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadEnderDragonFromNBT     (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadEndermanFromNBT        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadGhastFromNBT           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadGiantFromNBT           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadGuardianFromNBT        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadHorseFromNBT           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadIronGolemFromNBT       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadMagmaCubeFromNBT       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadMooshroomFromNBT       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadOcelotFromNBT          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadPigFromNBT             (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadRabbitFromNBT          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadSheepFromNBT           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadSilverfishFromNBT      (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadSkeletonFromNBT        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadSlimeFromNBT           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadSnowGolemFromNBT       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadSpiderFromNBT          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadSquidFromNBT           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadVillagerFromNBT        (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadWitchFromNBT           (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadWitherFromNBT          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadWolfFromNBT            (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadZombieFromNBT          (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadPigZombieFromNBT       (cEntityList & a_Entities, const cParsedNBT & a_NBT, int a_TagIdx);
	
	/** Loads the wolf's owner information from the NBT into the specified wolf entity. */
	void LoadWolfOwner(cWolf & a_Wolf, const cParsedNBT & a_NBT, int a_TagIdx);
	
	/// Loads entity common data from the NBT compound; returns true if successful
	bool LoadEntityBaseFromNBT(cEntity & a_Entity, const cParsedNBT & a_NBT, int a_TagIdx);
	
	/// Loads monster common data from the NBT compound; returns true if successful
	bool LoadMonsterBaseFromNBT(cMonster & a_Monster, const cParsedNBT & a_NBT, int a_TagIdx);
	
	/// Loads projectile common data from the NBT compound; returns true if successful
	bool LoadProjectileBaseFromNBT(cProjectileEntity & a_Entity, const cParsedNBT & a_NBT, int a_TagIx);
	
	/// Loads an array of doubles of the specified length from the specified NBT list tag a_TagIdx; returns true if successful
	bool LoadDoublesListFromNBT(double * a_Doubles, int a_NumDoubles, const cParsedNBT & a_NBT, int a_TagIdx);

	/// Loads an array of floats of the specified length from the specified NBT list tag a_TagIdx; returns true if successful
	bool LoadFloatsListFromNBT(float * a_Floats, int a_NumFloats, const cParsedNBT & a_NBT, int a_TagIdx);

	/// Helper function for extracting the X, Y, and Z int subtags of a NBT compound; returns true if successful
	bool GetBlockEntityNBTPos(const cParsedNBT & a_NBT, int a_TagIdx, int & a_X, int & a_Y, int & a_Z);
	
	/// Gets the correct MCA file either from cache or from disk, manages the m_MCAFiles cache; assumes m_CS is locked
	cMCAFile * LoadMCAFile(const cChunkCoords & a_Chunk);
	
	/// Copies a_Length bytes of data from the specified NBT Tag's Child into the a_Destination buffer
	void CopyNBTData(const cParsedNBT & a_NBT, int a_Tag, const AString & a_ChildName, char * a_Destination, size_t a_Length);
		
	// cWSSchema overrides:
	virtual bool LoadChunk(const cChunkCoords & a_Chunk) override;
	virtual bool SaveChunk(const cChunkCoords & a_Chunk) override;
	virtual const AString GetName(void) const override {return "anvil"; }
} ;




