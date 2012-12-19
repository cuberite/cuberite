
// WSSAnvil.h

// Interfaces to the cWSSAnvil class representing the Anvil world storage scheme




#pragma once

#include "WorldStorage.h"
#include "FastNBT.h"





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

	cWSSAnvil(cWorld * a_World);
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
		
		// Chunk timestamps, following the chunk headers, are unused by MCS
		
		/// Finds a free location large enough to hold a_Data. Gets a hint of the chunk coords, places the data there if it fits. Returns the sector number.
		unsigned FindFreeLocation(int a_LocalX, int a_LocalZ, const AString & a_Data);
		
		/// Opens a MCA file either for a Read operation (fails if doesn't exist) or for a Write operation (creates new if not found)
		bool OpenFile(bool a_IsForReading);
	} ;
	typedef std::list<cMCAFile *> cMCAFiles;
	
	cCriticalSection m_CS;
	cMCAFiles        m_Files;  // a MRU cache of MCA files

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
	
	/// Loads the chunk's biome map from vanilla-format; returns a_BiomeMap if biomes present and valid, NULL otherwise
	cChunkDef::BiomeMap * LoadVanillaBiomeMapFromNBT(cChunkDef::BiomeMap * a_BiomeMap, const cParsedNBT & a_NBT, int a_TagIdx);
	
	/// Loads the chunk's biome map from MCS format; returns a_BiomeMap if biomes present and valid, NULL otherwise
	cChunkDef::BiomeMap * LoadBiomeMapFromNBT(cChunkDef::BiomeMap * a_BiomeMap, const cParsedNBT & a_NBT, int a_TagIdx);
	
	/// Loads the chunk's entities from NBT data (a_Tag is the Level\\Entities list tag; may be -1)
	void LoadEntitiesFromNBT(cEntityList & a_Entitites, const cParsedNBT & a_NBT, int a_Tag);
	
	/// Loads the chunk's BlockEntities from NBT data (a_Tag is the Level\\TileEntities list tag; may be -1)
	void LoadBlockEntitiesFromNBT(cBlockEntityList & a_BlockEntitites, const cParsedNBT & a_NBT, int a_Tag);
	
	void LoadChestFromNBT     (cBlockEntityList & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadDispenserFromNBT (cBlockEntityList & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadFurnaceFromNBT   (cBlockEntityList & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadSignFromNBT      (cBlockEntityList & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadNoteFromNBT      (cBlockEntityList & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx);
	void LoadJukeboxFromNBT   (cBlockEntityList & a_BlockEntities, const cParsedNBT & a_NBT, int a_TagIdx);
	
	/// Helper function for extracting the X, Y, and Z int subtags of a NBT compound; returns true if successful
	bool GetBlockEntityNBTPos(const cParsedNBT & a_NBT, int a_TagIdx, int & a_X, int & a_Y, int & a_Z);
	
	/// Gets the correct MCA file either from cache or from disk, manages the m_MCAFiles cache; assumes m_CS is locked
	cMCAFile * LoadMCAFile(const cChunkCoords & a_Chunk);
	
	/// Copies a_Length bytes of data from the specified NBT Tag's Child into the a_Destination buffer
	void CopyNBTData(const cParsedNBT & a_NBT, int a_Tag, const AString & a_ChildName, char * a_Destination, int a_Length);
		
	// cWSSchema overrides:
	virtual bool LoadChunk(const cChunkCoords & a_Chunk) override;
	virtual bool SaveChunk(const cChunkCoords & a_Chunk) override;
	virtual const AString GetName(void) const override {return "anvil"; }
} ;




